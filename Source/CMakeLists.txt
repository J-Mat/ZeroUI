set(SourceDir "${ProjectRootDir}/Source")
set(CoreDir "${SourceDir}/Core")
set(UtilsDir "${SourceDir}/Utils")
set(TestDir "${SourceDir}/Test")
set(AssetsDir "${ProjectRootDir}/Assets")


add_subdirectory(Test)

file(COPY ${EngineAssetsDir}
DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})