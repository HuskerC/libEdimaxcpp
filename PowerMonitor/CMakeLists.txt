SET(sourcefiles
    powerMonitor.h
    powerMonitor.cpp
    main.cpp
    )

ADD_EXECUTABLE(PowerMonitor ${sourcefiles})
TARGET_LINK_LIBRARIES(PowerMonitor libEdimax)
TARGET_COMPILE_FEATURES(PowerMonitor PRIVATE cxx_range_for)
