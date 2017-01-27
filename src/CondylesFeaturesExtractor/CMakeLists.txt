
file(
	GLOB
	source_files
	*.[ch]pp
	*.[ch]xx
	*.[ch]
)

SEMMacroBuildCLI(
  NAME condylesfeaturesextractor
  ADDITIONAL_SRCS ${source_files}
  EXECUTABLE_ONLY
  TARGET_LIBRARIES ${ITK_LIBRARIES} ${VTK_LIBRARIES}
  )
