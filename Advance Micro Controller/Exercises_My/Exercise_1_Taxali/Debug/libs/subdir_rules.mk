################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
libs/LCD.obj: ../libs/LCD.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"E:/Installed Softwares/ti/ccsv7/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="E:/Installed Softwares/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/DELL/workspace_v7/Exercise_1" --include_path="E:/Installed Softwares/ti/ccsv7/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="libs/LCD.d_raw" --obj_directory="libs" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

libs/templateEMP.obj: ../libs/templateEMP.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"E:/Installed Softwares/ti/ccsv7/tools/compiler/ti-cgt-msp430_18.1.1.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="E:/Installed Softwares/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/DELL/workspace_v7/Exercise_1" --include_path="E:/Installed Softwares/ti/ccsv7/tools/compiler/ti-cgt-msp430_18.1.1.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="libs/templateEMP.d_raw" --obj_directory="libs" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


