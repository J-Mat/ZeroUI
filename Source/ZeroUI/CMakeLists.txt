set(ProjectName "ZeroUI")


configure_file(ZConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/ZConfig.h @ONLY)

ConstructSolutionDirTree( ${CMAKE_CURRENT_SOURCE_DIR} HeadList SrcList)

source_group(TREE ${ZeroUIDir} FILES ${HeadList} ${SrcList})

add_library(${ProjectName} STATIC ${HeadList} ${SrcList})

target_include_directories(${ProjectName} 
    PRIVATE "${ThirdPartyFolder}"
)

target_link_libraries(${ProjectName}
    PUBLIC stb_image
	PUBLIC D3D12MemAlloc
	PUBLIC spdlog
	PUBLIC crossguid
	PUBLIC glm
)


if (WIN32)
    target_link_libraries(${ProjectName}
		PUBLIC d3d12.lib
		PUBLIC dxgi.lib
		PUBLIC dxguid.lib
		PUBLIC d3dcompiler.lib
	)
endif()
