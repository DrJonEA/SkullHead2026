add_executable(${NAME}
        main.cpp
       	LedEye.cpp
       	StateEngine.cpp
       	SensorControl.cpp
        HeadControl.cpp
	 )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${NAME} 
	pico_stdlib 
	kw307
	hardware_i2c
	PicoLed
	picoServo)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 0)
pico_enable_stdio_uart(${NAME} 1)
target_compile_definitions(${NAME} PRIVATE
    PICO_DEFAULT_UART_RX_PIN=1
    PICO_DEFAULT_UART_TX_PIN=0
)
