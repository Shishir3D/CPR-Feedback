Aria Version:

    1.6.80.1
    aria-windows-sdk-net40\lib\win32-net40-vs2013, x86 & x64

    Updates:
        https://aria.microsoft.com/developer/start-now/using-the-aria-sdk/get-started/windows-managed
        https://aria.microsoft.com/documentation/content/articles/aria-sdks/aria-windows-sdk-net40.zip
        https://www.microsoft.com/en-us/download/details.aspx?id=40784 (VCRedist 2013)


This Aria version is a managed wrapper over a native C++ implementation, so it is built as x86 and x64. 

The Project Cordoba add-in is built as AnyCPU and thus will load as-is in both the x86 and x64 versions of Excel.exe. 

For the Aria dependency, we need to load the x86 dll into 32-bit Excel.exe, and the x64 dll into 64-bit Excel.exe.

Rather than split the Cordoba installer into 2 separate x86 and x64 installers, we are able to keep the AnyCPU version and installer and load the correct Aria dlls dynamically at runtime.

This is accomplished by hooking the AppDomain.AssemblyResolve() method, which is invoked at assembly load time when an assembly fails to load.

We've set up the projects to create the Aria dll load failure every time the add-in loads, no matter what version or bitness of Excel may be installed on the machine, so that we get this AssemblyResolve() callback and the opportunity to load the correct dll.

The setup and approach involves the following:

    - CordobaLib project references the Microsoft.Applications.Telemetry.Windows.proxy.dll. 
        
        This dll is a copy of the regular x86 Microsoft.Applications.Telemetry.Windows.dll, just renamed with the *proxy* suffix. 
        It is saved in the Aria subfolder of the add-in project and referenced from there. It is kept there along with all the other Aria binaries simply for management convenience to keep all the binaries in one place.
        The proxy dll is not not included in the add-in project and thus is never published or inclued in the installation. Thus, when the CordobaLib tries to load this *proxy* assembly at runtime, the assembly will never be found and the AssemblyResolve() callback will get invoked.

    - The Add-in project adds the Aria dlls and their VCRedist dependencies as Content files in the project.

        Adding the files as Content gets them included as part of the deployment manifest and package so that they get installed on users' machines.
        The Aria dlls were renamed to x86 and x64 and inclued in the root Aria folder, rather than keeping their origial names and placing them into x86 and x64 subfolders, because if they kept their original name, the ClickOnce build process would resolve the CordobaLib Aria dll dependency and would include the x86 version in the package root, outside of the Aria subfolder in the deployment package, which caused problems for getting AssemblyResolve() to work correctly on 32-bit versions of Excel.
        The Aria dlls have static links to 2 of the VCRedist binaries, as observed by the DependencyWalker tool:
          - MSVCR120.dll
          - MSVCP120.dll
        The VCRedist binaries were obtained from the Download Center link above. x86 & x64 packages were installed on a clean machine and these 2 binaries were plucked out of the %WINDIR%\SysWow64 and %WINDIR%\System32 directories respectively and included in the project.
        The VCRedist 2013 version of the Aria dlls was chosen becuase it is less likely that this older VCRedist will ever need or get another update, so the static install of these binaries into our own install directory where they will never be updated just felt safer. The 2015 binaries are on Update 3, so it may be that they could be further updated. They also appeared to utilize more binaries linkages (msvcp140.dll, vcruntime140.dll & ucrtbase.dll) that would need to be deployed, and also a potential dependency on the Universal CRT, all of which didn't seem worth investigating further since 2013 worked.

    - The Add-in code is modified to hook the AppDomain.AssemblyResolve() callback in order to load the Aria dlls dynamically.

        The AssemblyResolve() handler will load the correct Aria dll when requested. It determines the bitness of the current Excel.exe process running the add-in and chooses the correct Aria dll to load.
        Before it loads the Aria.dll, it updates the Excel.exe %PATH% environment variable to the location of the VCRedist binaries so that when the Aria dll loads and requires those, the loader will have the correct search path to find those binaries.

    - All of this was tested on a clean Win 7 machine with all 4 versions of Excel 2013/2016 x86/x64.
