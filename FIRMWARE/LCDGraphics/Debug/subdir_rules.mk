################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
LCD.obj: ../LCD.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 -me --include_path="C:/Users/Riya Simon/workspace_v8/LCDGraphics" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="LCD.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

communication.obj: ../communication.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 -me --include_path="C:/Users/Riya Simon/workspace_v8/LCDGraphics" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="communication.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

graphicsClasses.obj: ../graphicsClasses.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 -me --include_path="C:/Users/Riya Simon/workspace_v8/LCDGraphics" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="graphicsClasses.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 -me --include_path="C:/Users/Riya Simon/workspace_v8/LCDGraphics" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

pinmux.obj: ../pinmux.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/bin/armcl" -mv7M4 --code_state=16 -me --include_path="C:/Users/Riya Simon/workspace_v8/LCDGraphics" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.2.LTS/include" --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="pinmux.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


