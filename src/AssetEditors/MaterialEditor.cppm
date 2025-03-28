//
// Created by Vinnie on 28/03/2025.
//
module;
#include "ReflectionMacros.h"

export module Editor.MaterialEditor;
import Editor.AssetEditorRegistry;
import Engine.Node;
import Engine.Resource.Material;
import Engine.Resource;
import Editor.AssetEditor;

export class MaterialEditor : public AssetEditor{
  CRAB_CLASS(MaterialEditor, AssetEditor);

  inline static AssetEditorAutoRegister<MaterialEditor, MaterialResource> AutoReg;

public:
  void Update(float dt) override;
  void DrawGUI() override;
  void Ready() override;

};
