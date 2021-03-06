# Microsoft `spawnve()` issue

## The problem

It seems that the implementation of `spawnve()` in the modern Microsoft
UCRT API has a problem, and in certain conditions, calling the `spawn*e()`
functions with a non-NULL environment, crashes with _Error -1073741819_
which is 0xC0000005, or ERROR_ACCESS_DENIED.

These conditions are met by **GNU make**, and some programs invoked
by `make.exe` that try to create sub-processes with `spawn*e()`, fail.

Since `spawnve()` is also called by BusyBox in its `sh.exe` implementation,
this is a major issue, because it affects most common build tools.

## History

The problem was detected when attempting to rebuild the
[xPack Windows Build Tools](https://github.com/xpack-dev-tools/windows-build-tools-xpack)
package with a new build environment (xPack Build Box - XBB v3.3), which
upgraded the mingw-gcc compiler to use the UCRT API
([#19](https://github.com/xpack-dev-tools/windows-build-tools-xpack/issues/19)).

UCRT is the Microsoft
[Universal C Runtime](https://www.microsoft.com/en-us/download/details.aspx?id=48234),
and is available since Windows 7 SP1 from 2011.

The problem did not occur in the past, because `sh.exe` was built
to use the legacy MSVCRT API, which is not affected.

## Project overview

This open source project provides several test programs to diagnose
this problem. It is available from GitHub as
[xpack-dev-tools/ms-spawn-issue](https://github.com/xpack-dev-tools/ms-spawn-issue).

Some programs are avalable in binary form (`make.exe` and `ninja.exe`),
and some both as source code and in binary form, compiled with Microsoft
Visual Studio.

The `spawn-*.exe` binaries behave like a very simple shell used to
start a child process `dump-env.exe`.

- `spawn-env.exe` creates a new sub-process and passes the unmodified
environment, as received by `main()`.
- `spawn-null.exe` is identical to `spawn-env.exe`, except that it passes
a NULL pointer for the environment, which is a shortcut for inheriting
the full environment from the parent process.

The tests show the environment before the `spawn()` call
and in the child process.

## How to test

### Prerequisites

- a Windows 10 system or later.
- when using the Git repository, a Git client is needed, for example
[Git for Windows](https://git-scm.com/download/win).

### Download/clone the project

The tests can be downloaded either as a ZIP archive or as a full Git repository.

#### Download the ZIP archive

The latest version of this project can be downloaded as a ZIP archive from:

- <https://github.com/xpack-dev-tools/ms-spawn-issue/archive/refs/heads/master.zip>

It can be unpacked in any folder, for example in `tmp\ms-spawn-issue`.

#### Clone the full Git repository

The full Git repository can be cloned with:

```doscmd
C:\Users\ilg>mkdir tmp
C:\Users\ilg>cd tmp
C:\Users\ilg\tmp>git clone https://github.com/xpack-dev-tools/ms-spawn-issue.git
Cloning into 'ms-spawn-issue'...
remote: Enumerating objects: 114, done.
remote: Counting objects: 100% (114/114), done.
remote: Compressing objects: 100% (74/74), done.
Receiving objects:  85% (97/114)used 92 (delta 36), pack-reused 0R
Receiving objects: 100% (114/114), 468.93 KiB | 3.47 MiB/s, done.
Resolving deltas: 100% (54/54), done.

C:\Users\ilg\tmp>
```

### PowerShell

The following tests were executed in a `cmd.exe` terminal, but behave
the same in a PowerShell terminal. The only difference is that the
tests should be started with a relative path, like `.\make`.

### Run the `make` test

Change to the `ms-spawn-issue\tests` folder and run the tests:

```doscmd
C:\Users\ilg\tmp>cd ms-spawn-issue\tests

C:\Users\ilg\tmp\ms-spawn-issue\tests>make
spawn-env -c "dump-env one two"

Spawn test:
spawn env[0]='ALLUSERSPROFILE=C:\ProgramData'
spawn env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
spawn env[2]='CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files'
spawn env[3]='CommonProgramFiles=C:\Program Files\Common Files'
spawn env[4]='CommonProgramW6432=C:\Program Files\Common Files'
spawn env[5]='COMPUTERNAME=ILG-W11-TEST'
spawn env[6]='ComSpec=C:\Windows\system32\cmd.exe'
spawn env[7]='DriverData=C:\Windows\System32\Drivers\DriverData'
spawn env[8]='HOMEDRIVE=C:'
spawn env[9]='HOMEPATH=\Users\ilg'
spawn env[10]='LOCALAPPDATA=C:\Users\ilg\AppData\Local'
spawn env[11]='LOGONSERVER=\\ILG-W11-TEST'
spawn env[12]='MAKE_TERMERR=true'
spawn env[13]='MAKE_TERMOUT=true'
spawn env[14]='MAKEFLAGS='
spawn env[15]='MAKELEVEL=1'
spawn env[16]='MFLAGS='
spawn env[17]='NUMBER_OF_PROCESSORS=4'
spawn env[18]='OS=Windows_NT'
spawn env[19]='PATH=C:\Program Files (x86)\Parallels\Parallels Tools\Applications;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\nodejs\;C:\Program Files\Git\cmd;C:\Users\ilg\AppData\Local\Microsoft\WindowsApps;C:\Users\ilg\AppData\Roaming\npm;C:\Users\ilg\AppData\Local\Programs\Microsoft VS Code\bin'
spawn env[20]='PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC'
spawn env[21]='PROCESSOR_ARCHITECTURE=AMD64'
spawn env[22]='PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 158 Stepping 10, GenuineIntel'
spawn env[23]='PROCESSOR_LEVEL=6'
spawn env[24]='PROCESSOR_REVISION=9e0a'
spawn env[25]='ProgramData=C:\ProgramData'
spawn env[26]='ProgramFiles(x86)=C:\Program Files (x86)'
spawn env[27]='ProgramFiles=C:\Program Files'
spawn env[28]='ProgramW6432=C:\Program Files'
spawn env[29]='PROMPT=$P$G'
spawn env[30]='PSModulePath=C:\Program Files\WindowsPowerShell\Modules;C:\Windows\system32\WindowsPowerShell\v1.0\Modules'
spawn env[31]='PUBLIC=C:\Users\Public'
spawn env[32]='SESSIONNAME=Console'
spawn env[33]='SystemDrive=C:'
spawn env[34]='SystemRoot=C:\Windows'
spawn env[35]='TEMP=C:\Users\ilg\AppData\Local\Temp'
spawn env[36]='TMP=C:\Users\ilg\AppData\Local\Temp'
spawn env[37]='USERDOMAIN_ROAMINGPROFILE=ILG-W11-TEST'
spawn env[38]='USERDOMAIN=ILG-W11-TEST'
spawn env[39]='USERNAME=ilg'
spawn env[40]='USERPROFILE=C:\Users\ilg'
spawn env[41]='windir=C:\Windows'
spawn argv[0]='spawn-env'
spawn argv[1]='-c'
spawn argv[2]='dump-env one two'
spawn command 'dump-env.exe'...
make: *** [makefile:3: all] Error -1073741819

C:\Users\ilg\tmp\ms-spawn-issue\tests>
```

This test, which passes the environment received by `main()` fails
exactly inside the `_spawnvpe()` call.

```doscon
C:\Users\ilg\tmp\ms-spawn-issue\tests>make null
spawn-null -c "dump-env three four"

Spawn test:
spawn env[0]='ALLUSERSPROFILE=C:\ProgramData'
spawn env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
spawn env[2]='CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files'
spawn env[3]='CommonProgramFiles=C:\Program Files\Common Files'
spawn env[4]='CommonProgramW6432=C:\Program Files\Common Files'
spawn env[5]='COMPUTERNAME=ILG-W11-TEST'
spawn env[6]='ComSpec=C:\Windows\system32\cmd.exe'
spawn env[7]='DriverData=C:\Windows\System32\Drivers\DriverData'
spawn env[8]='HOMEDRIVE=C:'
spawn env[9]='HOMEPATH=\Users\ilg'
spawn env[10]='LOCALAPPDATA=C:\Users\ilg\AppData\Local'
spawn env[11]='LOGONSERVER=\\ILG-W11-TEST'
spawn env[12]='MAKE_TERMERR=true'
spawn env[13]='MAKE_TERMOUT=true'
spawn env[14]='MAKEFLAGS='
spawn env[15]='MAKELEVEL=1'
spawn env[16]='MFLAGS='
spawn env[17]='NUMBER_OF_PROCESSORS=4'
spawn env[18]='OS=Windows_NT'
spawn env[19]='PATH=C:\Program Files (x86)\Parallels\Parallels Tools\Applications;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\nodejs\;C:\Program Files\Git\cmd;C:\Users\ilg\AppData\Local\Microsoft\WindowsApps;C:\Users\ilg\AppData\Roaming\npm;C:\Users\ilg\AppData\Local\Programs\Microsoft VS Code\bin'
spawn env[20]='PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC'
spawn env[21]='PROCESSOR_ARCHITECTURE=AMD64'
spawn env[22]='PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 158 Stepping 10, GenuineIntel'
spawn env[23]='PROCESSOR_LEVEL=6'
spawn env[24]='PROCESSOR_REVISION=9e0a'
spawn env[25]='ProgramData=C:\ProgramData'
spawn env[26]='ProgramFiles(x86)=C:\Program Files (x86)'
spawn env[27]='ProgramFiles=C:\Program Files'
spawn env[28]='ProgramW6432=C:\Program Files'
spawn env[29]='PROMPT=$P$G'
spawn env[30]='PSModulePath=C:\Program Files\WindowsPowerShell\Modules;C:\Windows\system32\WindowsPowerShell\v1.0\Modules'
spawn env[31]='PUBLIC=C:\Users\Public'
spawn env[32]='SESSIONNAME=Console'
spawn env[33]='SystemDrive=C:'
spawn env[34]='SystemRoot=C:\Windows'
spawn env[35]='TEMP=C:\Users\ilg\AppData\Local\Temp'
spawn env[36]='TMP=C:\Users\ilg\AppData\Local\Temp'
spawn env[37]='USERDOMAIN_ROAMINGPROFILE=ILG-W11-TEST'
spawn env[38]='USERDOMAIN=ILG-W11-TEST'
spawn env[39]='USERNAME=ilg'
spawn env[40]='USERPROFILE=C:\Users\ilg'
spawn env[41]='windir=C:\Windows'
spawn argv[0]='spawn-null'
spawn argv[1]='-c'
spawn argv[2]='dump-env three four'
spawn command 'dump-env.exe'...

Dump process environment:
envp[0]='ALLUSERSPROFILE=C:\ProgramData'
envp[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
envp[2]='CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files'
envp[3]='CommonProgramFiles=C:\Program Files\Common Files'
envp[4]='CommonProgramW6432=C:\Program Files\Common Files'
envp[5]='COMPUTERNAME=ILG-W11-TEST'
envp[6]='ComSpec=C:\Windows\system32\cmd.exe'
envp[7]='DriverData=C:\Windows\System32\Drivers\DriverData'
envp[8]='HOMEDRIVE=C:'
envp[9]='HOMEPATH=\Users\ilg'
envp[10]='LOCALAPPDATA=C:\Users\ilg\AppData\Local'
envp[11]='LOGONSERVER=\\ILG-W11-TEST'
envp[12]='MAKE_TERMERR=true'
envp[13]='MAKE_TERMOUT=true'
envp[14]='MAKEFLAGS='
envp[15]='MAKELEVEL=1'
envp[16]='MFLAGS='
envp[17]='NUMBER_OF_PROCESSORS=4'
envp[18]='OS=Windows_NT'
envp[19]='PATH=C:\Program Files (x86)\Parallels\Parallels Tools\Applications;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\nodejs\;C:\Program Files\Git\cmd;C:\Users\ilg\AppData\Local\Microsoft\WindowsApps;C:\Users\ilg\AppData\Roaming\npm;C:\Users\ilg\AppData\Local\Programs\Microsoft VS Code\bin'
envp[20]='PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC'
envp[21]='PROCESSOR_ARCHITECTURE=AMD64'
envp[22]='PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 158 Stepping 10, GenuineIntel'
envp[23]='PROCESSOR_LEVEL=6'
envp[24]='PROCESSOR_REVISION=9e0a'
envp[25]='ProgramData=C:\ProgramData'
envp[26]='ProgramFiles(x86)=C:\Program Files (x86)'
envp[27]='ProgramFiles=C:\Program Files'
envp[28]='ProgramW6432=C:\Program Files'
envp[29]='PROMPT=$P$G'
envp[30]='PSModulePath=C:\Program Files\WindowsPowerShell\Modules;C:\Windows\system32\WindowsPowerShell\v1.0\Modules'
envp[31]='PUBLIC=C:\Users\Public'
envp[32]='SESSIONNAME=Console'
envp[33]='SystemDrive=C:'
envp[34]='SystemRoot=C:\Windows'
envp[35]='TEMP=C:\Users\ilg\AppData\Local\Temp'
envp[36]='TMP=C:\Users\ilg\AppData\Local\Temp'
envp[37]='USERDOMAIN_ROAMINGPROFILE=ILG-W11-TEST'
envp[38]='USERDOMAIN=ILG-W11-TEST'
envp[39]='USERNAME=ilg'
envp[40]='USERPROFILE=C:\Users\ilg'
envp[41]='windir=C:\Windows'
argv[0]='dump-env'
argv[1]='three'
argv[2]='four'
spawn ret=0 errno=0

C:\Users\ilg\tmp\ms-spawn-issue\tests>
```

This test, which passes a NULL pointer as environment, passes.

Starting exactly the same command that failed when invoked from make,
but this time from a terminal, works as expected:

```doscmd
C:\Users\ilg\tmp\ms-spawn-issue\tests>spawn-env -c "dump-env one two"

Spawn test:
spawn env[0]='ALLUSERSPROFILE=C:\ProgramData'
spawn env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
spawn env[2]='CommonProgramFiles=C:\Program Files\Common Files'
spawn env[3]='CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files'
spawn env[4]='CommonProgramW6432=C:\Program Files\Common Files'
spawn env[5]='COMPUTERNAME=ILG-W11-TEST'
spawn env[6]='ComSpec=C:\Windows\system32\cmd.exe'
spawn env[7]='DriverData=C:\Windows\System32\Drivers\DriverData'
spawn env[8]='HOMEDRIVE=C:'
spawn env[9]='HOMEPATH=\Users\ilg'
spawn env[10]='LOCALAPPDATA=C:\Users\ilg\AppData\Local'
spawn env[11]='LOGONSERVER=\\ILG-W11-TEST'
spawn env[12]='NUMBER_OF_PROCESSORS=4'
spawn env[13]='OS=Windows_NT'
spawn env[14]='Path=C:\Program Files (x86)\Parallels\Parallels Tools\Applications;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\nodejs\;C:\Program Files\Git\cmd;C:\Users\ilg\AppData\Local\Microsoft\WindowsApps;C:\Users\ilg\AppData\Roaming\npm;C:\Users\ilg\AppData\Local\Programs\Microsoft VS Code\bin'
spawn env[15]='PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC'
spawn env[16]='PROCESSOR_ARCHITECTURE=AMD64'
spawn env[17]='PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 158 Stepping 10, GenuineIntel'
spawn env[18]='PROCESSOR_LEVEL=6'
spawn env[19]='PROCESSOR_REVISION=9e0a'
spawn env[20]='ProgramData=C:\ProgramData'
spawn env[21]='ProgramFiles=C:\Program Files'
spawn env[22]='ProgramFiles(x86)=C:\Program Files (x86)'
spawn env[23]='ProgramW6432=C:\Program Files'
spawn env[24]='PROMPT=$P$G'
spawn env[25]='PSModulePath=C:\Program Files\WindowsPowerShell\Modules;C:\Windows\system32\WindowsPowerShell\v1.0\Modules'
spawn env[26]='PUBLIC=C:\Users\Public'
spawn env[27]='SESSIONNAME=Console'
spawn env[28]='SystemDrive=C:'
spawn env[29]='SystemRoot=C:\Windows'
spawn env[30]='TEMP=C:\Users\ilg\AppData\Local\Temp'
spawn env[31]='TMP=C:\Users\ilg\AppData\Local\Temp'
spawn env[32]='USERDOMAIN=ILG-W11-TEST'
spawn env[33]='USERDOMAIN_ROAMINGPROFILE=ILG-W11-TEST'
spawn env[34]='USERNAME=ilg'
spawn env[35]='USERPROFILE=C:\Users\ilg'
spawn env[36]='windir=C:\Windows'
spawn argv[0]='spawn-env'
spawn argv[1]='-c'
spawn argv[2]='dump-env one two'
spawn command 'dump-env.exe'...

Dump process environment:
envp[0]='ALLUSERSPROFILE=C:\ProgramData'
envp[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
envp[2]='CommonProgramFiles=C:\Program Files\Common Files'
envp[3]='CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files'
envp[4]='CommonProgramW6432=C:\Program Files\Common Files'
envp[5]='COMPUTERNAME=ILG-W11-TEST'
envp[6]='ComSpec=C:\Windows\system32\cmd.exe'
envp[7]='DriverData=C:\Windows\System32\Drivers\DriverData'
envp[8]='HOMEDRIVE=C:'
envp[9]='HOMEPATH=\Users\ilg'
envp[10]='LOCALAPPDATA=C:\Users\ilg\AppData\Local'
envp[11]='LOGONSERVER=\\ILG-W11-TEST'
envp[12]='NUMBER_OF_PROCESSORS=4'
envp[13]='OS=Windows_NT'
envp[14]='Path=C:\Program Files (x86)\Parallels\Parallels Tools\Applications;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\nodejs\;C:\Program Files\Git\cmd;C:\Users\ilg\AppData\Local\Microsoft\WindowsApps;C:\Users\ilg\AppData\Roaming\npm;C:\Users\ilg\AppData\Local\Programs\Microsoft VS Code\bin'
envp[15]='PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC'
envp[16]='PROCESSOR_ARCHITECTURE=AMD64'
envp[17]='PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 158 Stepping 10, GenuineIntel'
envp[18]='PROCESSOR_LEVEL=6'
envp[19]='PROCESSOR_REVISION=9e0a'
envp[20]='ProgramData=C:\ProgramData'
envp[21]='ProgramFiles=C:\Program Files'
envp[22]='ProgramFiles(x86)=C:\Program Files (x86)'
envp[23]='ProgramW6432=C:\Program Files'
envp[24]='PROMPT=$P$G'
envp[25]='PSModulePath=C:\Program Files\WindowsPowerShell\Modules;C:\Windows\system32\WindowsPowerShell\v1.0\Modules'
envp[26]='PUBLIC=C:\Users\Public'
envp[27]='SESSIONNAME=Console'
envp[28]='SystemDrive=C:'
envp[29]='SystemRoot=C:\Windows'
envp[30]='TEMP=C:\Users\ilg\AppData\Local\Temp'
envp[31]='TMP=C:\Users\ilg\AppData\Local\Temp'
envp[32]='USERDOMAIN=ILG-W11-TEST'
envp[33]='USERDOMAIN_ROAMINGPROFILE=ILG-W11-TEST'
envp[34]='USERNAME=ilg'
envp[35]='USERPROFILE=C:\Users\ilg'
envp[36]='windir=C:\Windows'
argv[0]='dump-env'
argv[1]='one'
argv[2]='two'
spawn ret=0 errno=0

C:\Users\ilg\tmp\ms-spawn-issue\tests>
```

### Run the `ninja` test

A similar test can be performed with the Ninja Build system instead of
GNU make.

The commands are similar:

```doscmd
C:\Users\ilg\tmp\ms-spawn-issue\tests>ninja
[1/1] spawn-env -c "dump-env one two"

Dump process environment:
envp[0]='ALLUSERSPROFILE=C:\ProgramData'
envp[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
envp[35]='USERPROFILE=C:\Users\ilg'
envp[36]='windir=C:\Windows'
argv[0]='dump-env'
argv[1]='one'
argv[2]='two'

Spawn test:
spawn env[0]='ALLUSERSPROFILE=C:\ProgramData'
spawn env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
spawn env[35]='USERPROFILE=C:\Users\ilg'
spawn env[36]='windir=C:\Windows'
spawn argv[0]='spawn-env'
spawn argv[1]='-c'
spawn argv[2]='dump-env one two'
spawn command 'dump-env.exe'...
spawn ret=0 errno=0

C:\Users\ilg\tmp\ms-spawn-issue\tests>ninja null
[1/1] spawn-null -c "dump-env three four"

Dump process environment:
envp[0]='ALLUSERSPROFILE=C:\ProgramData'
envp[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
envp[35]='USERPROFILE=C:\Users\ilg'
envp[36]='windir=C:\Windows'
argv[0]='dump-env'
argv[1]='three'
argv[2]='four'

Spawn test:
spawn env[0]='ALLUSERSPROFILE=C:\ProgramData'
spawn env[1]='APPDATA=C:\Users\ilg\AppData\Roaming'
...
spawn env[35]='USERPROFILE=C:\Users\ilg'
spawn env[36]='windir=C:\Windows'
spawn argv[0]='spawn-null'
spawn argv[1]='-c'
spawn argv[2]='dump-env three four'
spawn command 'dump-env.exe'...
spawn ret=0 errno=0

C:\Users\ilg\tmp\ms-spawn-issue\tests>
```

As it can be seen, both tests pass, so the problem is specific to `make.exe`;
the method used by Ninja to
create sub-processes is probably different and does not trigger the bug,
so projects using `ninja.exe` are not affected.

Note: At first sight, the output seems out of order, but in fact it
is even more ordered than for `make.exe`, since it shows the output
in dependency order, after each command completes.

## Source files

Both tests are based on the same source code (`sources/spawn/spawn.c`),
the first compiled to
forward the environment to the child process, then compiled to pass a NULL
pointer as environment, which is a shortcut to inherit the entire environment.

The `spawn-env.exe` and `spawn-null.exe` binaries were compiled with
Visual Studio 2022 on a Windows 11 machine, but with Platform Tools v142 and
the Windows 10 SDK, so the build should also be possible with Visual
Studio 2019 on Windows 10.

The VS solution (`sources/projects.sln`) and the project files are included.

## Conclusion

The current theory is that when Microsoft migrated from MSVCRT to UCRT
they changed the implementation and introduced a subtle bug that does not
show in the common use case.

However, `make.exe` uses an elaborate mechanism to create sub-processes, and
the context of the process running the `spawn-*.exe` is somehow different; this
difference triggers the bug when this process tries to spawn a new process
with an explicit environment.

## Suggestions

The issue is quite tricky, and it involves three parties, GNU make,
BusyBox and Microsoft. At any point, each party may try to shift the
blame to the other, but in fact all three should improve their code:

- **GNU make** should rewrite the code which creates sub-processes to run
  with the current Windows and BusyBox; Ninja Build can be a source of
  inspiration, since their code seems ok
- **BusyBox** should not rely on the unreliable `spawnve()` available in
  the new Windows releases, and reimplement it with a code that is
  functional with the current make and Windows
- **Microsoft** should fix their `spawnve()` code and prevent it from
  crashing when BusyBox is invoked from `make`

## Bug reports

The following tickets were opened:

- <https://github.com/rmyorston/busybox-w32/issues/234>
- <https://savannah.gnu.org/bugs/index.php?61409>

## Contributions

All contributions to clarify this issue will be highly appreciated.

Please use the GitHub
[Issues](https://github.com/xpack-dev-tools/ms-spawn-issue/issues)
to post your findings.

Thank you,

Liviu
