set(ProjectName "ZeroUI")

ConstructSolutionDirTree( ${CMAKE_CURRENT_SOURCE_DIR} HeadList SrcList)

source_group(TREE ${TestDir} FILES ${HeadList} ${SrcList})

add_executable(${ProjectName} ${HeadList} ${SrcList})

#target_link_libraries(${ProjectName} PRIVATE Runtime)
#target_include_directories(${ProjectName}
#    PRIVATE "${EngineRuntimeDir}"
#    PRIVATE "${EngineIntermediateDir}"
#    PRIVATE "${EngineEditorDir}"
#)
