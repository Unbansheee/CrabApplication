using System.Reflection;
using System.Runtime.Loader;

namespace CrabEngine;

public class ScriptLoadContext : AssemblyLoadContext
{
    private AssemblyDependencyResolver _resolver;

    public ScriptLoadContext(string mainAssemblyToLoadPath) : base(isCollectible: true)
    {
        _resolver = new AssemblyDependencyResolver(mainAssemblyToLoadPath);
    }

    protected override Assembly Load(AssemblyName assemblyName)
    {
        var path = _resolver.ResolveAssemblyToPath(assemblyName);
        return path != null ? LoadFromAssemblyPath(path) : null;
    }
}

public class ScriptModule
{
    private ScriptLoadContext _context;
    private Assembly _loadedAssembly;
    private string _libName;
    
    private static readonly List<Type> _scriptTypes = new();

    public string AssemblyPath { get; }

    public ScriptModule(ScriptLoadContext context, Assembly assembly, string assemblyPath, string assemblyName)
    {
        _context = context;
        _loadedAssembly = assembly;
        AssemblyPath = assemblyPath;
        _libName = assemblyName;
        
        var scriptTypes = _loadedAssembly.GetTypes()
            .Where(t => typeof(Object).IsAssignableFrom(t) && !t.IsInterface && !t.IsAbstract)
            .Where(t => t.GetCustomAttributes(typeof(NativeEngineTypeAttribute), false).Length == 0);
        
        foreach (var type in scriptTypes)
        {
            _scriptTypes.Add(type);
            System.Console.WriteLine("[C# Engine] Registered script: " + type.Name);
        }
    }

    public void InvokeEntryPoint()
    {
        var entry = _loadedAssembly.GetType(_libName + ".EntryPoint")?.GetMethod("Init");
        entry?.Invoke(null, null);
        
    }
    
    public List<Type> GetScriptTypes()
    {
        return _scriptTypes;
    }
    
    public void Unload()
    {
        _context?.Unload();
        _context = null;
        _loadedAssembly = null;

        GC.Collect();
        GC.WaitForPendingFinalizers();
    }
}

public static class ScriptEngine
{
    public static ScriptModule? LoadScriptAssembly(string path, string libName)
    {
        var context = new ScriptLoadContext(path);
        Assembly? assembly = null;
        try
        {
            assembly = context.LoadFromAssemblyPath(path);
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            return null;
        }
        
        return new ScriptModule(context, assembly, path, libName);
    }
}