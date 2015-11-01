SET(sourcefiles
    deviceMonitor.h
    deviceMonitor.cpp
    main.cpp
    )

ADD_EXECUTABLE(DeviceMonitor ${sourcefiles})
TARGET_LINK_LIBRARIES(DeviceMonitor libEdimax)
TARGET_COMPILE_FEATURES(DeviceMonitor PRIVATE cxx_range_for)
