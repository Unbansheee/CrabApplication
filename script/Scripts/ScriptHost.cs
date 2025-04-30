using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

namespace Scripts;

[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
public struct ScriptInfo
{
    public nint Name; // char*
    public nint ParentClassName; // char*
}


public static class ScriptHost
{
    private static readonly List<Type> _scriptTypes = new();

    [UnmanagedCallersOnly(EntryPoint = "RegisterAllScripts")]
    public static void RegisterAllScripts()
    {
        var scriptTypes = AppDomain.CurrentDomain.GetAssemblies()
            .SelectMany(a => a.GetTypes())
            .Where(t => typeof(Scripts.Object).IsAssignableFrom(t) && !t.IsInterface && !t.IsAbstract)
            .Where(t => t.GetCustomAttributes(typeof(NativeEngineTypeAttribute), false).Length == 0);

        foreach (var type in scriptTypes)
        {
            _scriptTypes.Add(type);
            System.Console.WriteLine("Initialized script: " + type.Name);
        }
    }

    [UnmanagedCallersOnly(EntryPoint = "GetScriptCount")]
    public static int GetScriptCount()
    {
        return _scriptTypes.Count;
    }
    
    [UnmanagedCallersOnly(EntryPoint = "PrintTransform")]
    public static void PrintTransform(Transform t)
    {
        Console.WriteLine(t);
    }
    
    [UnmanagedCallersOnly(EntryPoint = "GetScriptInfoList")]
    public static nint GetScriptInfoList()
    {
        var types = _scriptTypes;

        var list = new List<ScriptInfo>();
        foreach (var t in types)
        {
            var nameBytes = Encoding.ASCII.GetBytes(t.FullName! + '\0');
            var ptr = Marshal.AllocHGlobal(nameBytes.Length);
            Marshal.Copy(nameBytes, 0, ptr, nameBytes.Length);

            bool parentIsNative =
                (t.BaseType?.GetCustomAttributes(typeof(NativeEngineTypeAttribute), false).Length != 0);
            var parentNameBytes = Encoding.ASCII.GetBytes(parentIsNative ? t.BaseType.Name! + '\0': t.BaseType.FullName! + '\0');
            var parentNamePtr = Marshal.AllocHGlobal(nameBytes.Length);
            Marshal.Copy(parentNameBytes, 0, parentNamePtr, parentNameBytes.Length);
            
            list.Add(new ScriptInfo
            {
                Name = ptr,
                ParentClassName = parentNamePtr
            });
        }

        var bufferSize = list.Count * Marshal.SizeOf<ScriptInfo>();
        var buffer = Marshal.AllocHGlobal(bufferSize);
        for (int i = 0; i < list.Count; ++i)
            Marshal.StructureToPtr(list[i], buffer + i * Marshal.SizeOf<ScriptInfo>(), false);

        return buffer;
    }
    
    [UnmanagedCallersOnly(EntryPoint = "CreateScriptInstance")]
    public static unsafe IntPtr CreateScriptInstance(IntPtr nativeOwner, char* typeName) {
        string instanceType = Marshal.PtrToStringUni((IntPtr)typeName);
        Console.WriteLine("Trying to spawn a " + instanceType);
        var instance = Activator.CreateInstance(_scriptTypes.Find(type => type.FullName == instanceType) ??
                                                throw new InvalidOperationException(), nativeOwner) as Scripts.Object;
        
        
        GCHandle handle = GCHandle.Alloc(instance);
        return (IntPtr)handle;
    }
    
    [UnmanagedCallersOnly(EntryPoint = "RegisterNativeFunction")]
    public static unsafe void RegisterNativeFunction(char* typeName, char* functionName, IntPtr fn) {
        string instanceTypeName = Marshal.PtrToStringUni((IntPtr)typeName);
        string fnName = Marshal.PtrToStringUni((IntPtr)functionName);

        Console.WriteLine($"Trying to register function {fnName} on Class {instanceTypeName}");
        var type = AppDomain.CurrentDomain.GetAssemblies()
            .SelectMany(a => a.GetTypes())
            .FirstOrDefault(t => t.FullName == instanceTypeName);
        
        if (fn == IntPtr.Zero)
            throw new InvalidOperationException($"No native function found for method '{fnName}'");
        
        foreach (var field in type.GetFields(BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic))
        {
            var attr = field.GetCustomAttribute<NativeBindAttribute>();
            if (attr == null)
                continue;

            if (attr.FunctionName != fnName)
            {
                continue;
            }
            
            var del = Marshal.GetDelegateForFunctionPointer(fn, field.FieldType);
            field.SetValue(null, del);

            Console.WriteLine($"Bound native method '{fnName}' to field '{field.Name}'");
        }
    }
    
    
    [UnmanagedCallersOnly(EntryPoint = "CallScriptMethod")]
    public static unsafe void CallScriptMethod(IntPtr handle, char* methodName, void** args, int argCount, void* returnBuffer)
    {
        string name = Marshal.PtrToStringUni((IntPtr)methodName)!;
        object? instance = GCHandle.FromIntPtr(handle).Target;
        
        var method = instance!.GetType().GetMethod(name, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance);
        if (method == null) throw new MissingMethodException(name);

        var parameters = method.GetParameters();
        var convertedArgs = new object?[argCount];
        for (int i = 0; i < argCount; i++) {
            var paramType = parameters[i].ParameterType;
            convertedArgs[i] = Marshal.PtrToStructure(new IntPtr(args[i]), paramType);
        }

        object? result = method.Invoke(instance, convertedArgs);
        if (returnBuffer != null && result != null) {
            Marshal.StructureToPtr(result, new IntPtr(returnBuffer), false);
        }
    }
    
    [UnmanagedCallersOnly(EntryPoint = "DestroyScript")]
    public static void DestroyScript(IntPtr handle) {
        var gch = GCHandle.FromIntPtr(handle);
        gch.Free();
    }
}

