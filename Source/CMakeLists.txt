set(SourceDir "${ProjectRootDir}/Source")
set(ZeroUIDir "${SourceDir}/ZeroUI")
set(TestDir "${SourceDir}/Test")
set(AssetsDir "${ProjectRootDir}/Assets")


add_subdirectory(Test)
add_subdirectory(ZeroUI)

file(COPY ${EngineAssetsDir}
DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})