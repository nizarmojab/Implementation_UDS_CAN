# Implementation_UDS_CAN

## Description
This project implements the Unified Diagnostic Services (UDS) protocol on an STM32 Nucleo F446RE microcontroller, utilizing CAN (Controller Area Network) communication for diagnostics and UART communication for testing and debugging. UDS, defined by the ISO 14229-1 standard, is widely used for diagnostics and maintenance of Electronic Control Units (ECUs) in automotive systems.

## Features
The project covers multiple UDS services organized into functional units. Each service has been implemented and tested, enabling robust fault detection, system configuration, and ECU reprogramming capabilities.

### Diagnostic Communication Management Functional Unit
- Diagnostic Session Control (0x10)
- ECU Reset (0x11)
- Security Access (0x27)
- Communication Control (0x28)
- Tester Present (0x3E)
- Access Timing Parameter (0x83)
- Secured Data Transmission (0x84)
- Control DTC Setting (0x85)
- Response on Event (0x86)
- Link Control (0x87)

### Data Transmission Functional Unit
- Read Data by Identifier (0x22)
- Read Memory by Address (0x23)
- Read Data by Periodic Identifier (0x2A)
- Dynamically Define Data Identifier (0x2C)
- Write Data By Identifier (0x2E)

### Stored Data Transmission Functional Unit
- Clear Diagnostic Information (0x14)
- Read DTC Information Service (0x19)

### InputOutput Control Functional Unit
- InputOutputControlByIdentifier (0x2F)

### Routine Functional Unit
- Routine Control (0x31)

### Upload Download Functional Unit
- Request Download (0x34)
- Request Upload (0x35)
- Transfer Data (0x36)
- Request Transfer Exit (0x37)
- Request File Transfer (0x38)

## Project Structure
The project is organized into **Inc** and **Src** folders for header (.h) and implementation (.c) files, as well as a **Test_uds_services** folder containing Python scripts for testing each service.

### Inc
Contains header files for the UDS functional units.
- `Data_Transmission_functional_unit.h`
- `Diagnostic_Communication_Management_functional_unit.h`
- `InputOutput_Control_functional_unit.h`
- `Routine_functional_unit.h`
- `Stored_Data_Transmission_functional_unit.h`
- `Upload_Download_functional_unit.h`
- `uds_services.h`: General declarations for UDS services.

### Src
Contains the implementations of UDS services.
- `Data_Transmission_functional_unit.c`
- `Diagnostic_Communication_Management_functional_unit.c`
- `InputOutput_Control_functional_unit.c`
- `Routine_functional_unit.c`
- `Stored_Data_Transmission_functional_unit.c`
- `Upload_Download_functional_unit.c`
- `uds_services.c`: Manages the main UDS services.

### Test_uds_services
Contains Python scripts for unit testing each UDS service.

## Requirements
- **Eclipse** for embedded code development.
- **Python** to execute test scripts.
- **CAN Interface** for CAN communication.
- **UART Interface** for debugging and tests.

