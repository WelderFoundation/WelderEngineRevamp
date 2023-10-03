// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class MeshProcessor
{
public:
  MeshProcessor(MeshBuilder* meshBuilder, MeshDataMap& meshDataMap, MaterialDataMap& materialDataMap);
  ~MeshProcessor();

  void SetupTransformationMatricies();
  void ExtractMaterialData(const aiScene* scene);
  void ExtractAndProcessMeshData(const aiScene* scene);
  void ExportMeshData(String outputPath);

  void WriteSingleMeshes(String outputPath);
  void WriteCombinedMesh(String outputPath);

  MeshBuilder* mBuilder;

  MeshDataMap& mMeshDataMap;

  MaterialDataMap& mMaterialDataMap;
};

} // namespace Zero
