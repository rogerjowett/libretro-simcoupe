// Part of SimCoupe - A SAM Coupe emulator
//
// IO.h: SAM I/O port handling
//
//  Copyright (c) 1999-2012 Simon Owen
//  Copyright (c) 1996-2001 Allan Skillman
//  Copyright (c) 2000-2001 Dave Laundon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef IO_H
#define IO_H

typedef struct {
    BYTE bRed, bGreen, bBlue;
} COLOUR;

enum { AUTOLOAD_NONE, AUTOLOAD_DISK, AUTOLOAD_TAPE };


class IO
{
    public:
        static bool Init (bool fFirstInit_=false);
        static void Exit (bool fReInit_=false);

        static BYTE In (WORD wPort_);
        static void Out (WORD wPort_, BYTE bVal_);

        static void OutLmpr (BYTE bVal_);
        static void OutHmpr (BYTE bVal_);
        static void OutVmpr (BYTE bVal_);
        static void OutLepr (BYTE bVal_);
        static void OutHepr (BYTE bVal_);
        static void OutClut (WORD wPort_, BYTE bVal_);

        static void FrameUpdate ();
        static void UpdateInput();
        static const COLOUR *GetPalette ();
        static bool IsAtStartupScreen (bool fExit_=false);
        static void AutoLoad (int nType_, bool fOnlyAtStartup_=true);
        static void WakeAsic ();

        static bool EiHook ();
        static bool Rst8Hook ();
        static bool Rst48Hook ();
};


class CIoDevice
{
    public:
        virtual ~CIoDevice () { }

    public:
        virtual void Reset () { }

        virtual BYTE In (WORD wPort_) { return 0xff; }
        virtual void Out (WORD wPort_, BYTE bVal_) { }

        virtual void FrameEnd () { }

        virtual void LoadState (const char *pcszFile_) { }  // preserve basic state (such as NVRAM)
        virtual void SaveState (const char *pcszFile_) { }
};

enum { drvNone, drvFloppy, drvAtom, drvAtomLite, drvSDIDE };

class CDiskDevice :  public CIoDevice
{
    public:
        CDiskDevice () : m_uActive(0) { }
        ~CDiskDevice () { }

    public:
        void Reset () { }
        void FrameEnd () { if (m_uActive) m_uActive--; }

    public:
        virtual bool Insert (const char* pcszImage_, bool fAutoLoad_=false) { return false; }
        virtual void Eject () { }
        virtual bool Save () { return true; }

    public:
        virtual const char* DiskPath () const = 0;
        virtual const char* DiskFile () const = 0;

        virtual bool HasDisk () const { return false; }
        virtual bool DiskModified () const { return false; }
        virtual bool IsLightOn () const { return false; }
        virtual bool IsActive () const { return m_uActive != 0; }

        virtual void SetDiskModified (bool fModified_=true) { }

    protected:
        UINT m_uActive; // active when non-zero, decremented by FrameEnd()
};

#define in_byte     IO::In
#define out_byte    IO::Out


#define LEPR_PORT           128
#define HEPR_PORT           129

#define CLOCK_PORT          239         // SAMBUS and DALLAS clocks

#define PEN_MASK            0x1f8
#define LPEN_PORT           248         // Input
#define HPEN_PORT           504         // Input
#define CLUT_BASE_PORT      248         // Output

#define STATUS_PORT         249         // Input
#define LINE_PORT           249         // Output

#define LMPR_PORT           250
#define HMPR_PORT           251
#define VMPR_PORT           252
#define MIDI_PORT           253

#define KEYBOARD_PORT       254         // Input
#define BORDER_PORT         254         // Output

#define SOUND_MASK          0x1ff
#define SOUND_DATA          255         // Output (register select)
#define SOUND_ADDR          511         // Output (data)
#define ATTR_PORT           255         // Input

#define KEMPSTON_PORT       31          // Kempston joystick

#define BLUE_ALPHA_PORT     127         // Blue Alpha Sampler and VoiceBox

#define QUAZAR_PORT         208         // Quazar Surround
#define SID_PORT            212         // Quazar SID interface at 0xD4xx

// Floppy drives or ATOM hard disk - 111d0srr : d = drive, s = side, r = register
#define FLOPPY_MASK         0xf8        // 11111000
#define FLOPPY1_BASE        224         // 224 to 231
#define FLOPPY2_BASE        240         // 240 to 247

#define PRINTL_MASK         0xfc        // 11111100
#define PRINTL_BASE         232         // 11101000
#define PRINTL1_DATA        232
#define PRINTL1_STAT        233
#define PRINTL2_DATA        234
#define PRINTL2_STAT        235

#define SERIAL_MASK         0xfe        // 11111110
#define SERIAL_BASE         236         // 11101100
#define SERIAL1             236
#define SERIAL2             237
// Did I hear 238 and 239 SERIAL3 and SERIAL4?  what about the clock port?

#define SDIDE_DATA          189
#define SDIDE_REG           191

#define LMPR_PAGE_MASK      0x1f
#define LMPR_PAGE           (lmpr & LMPR_PAGE_MASK)
#define LMPR_ROM0_OFF       0x20
#define LMPR_ROM1           0x40
#define LMPR_WPROT          0x80

#define HMPR_PAGE_MASK      0x1f
#define HMPR_MD3COL_MASK    0x60
#define HMPR_MCNTRL_MASK    0x80
#define HMPR_PAGE           (hmpr & HMPR_PAGE_MASK)
#define HMPR_MD3COL         (hmpr & HMPR_MD3_MASK)
#define HMPR_MCNTRL         (hmpr & HMPR_MCNTRL_MASK)

#define MODE_1              0x00
#define MODE_2              0x20
#define MODE_3              0x40
#define MODE_4              0x60

#define VMPR_PAGE_MASK      0x1f
#define VMPR_MODE_MASK      0x60
#define VMPR_MODE_SHIFT     5
#define VMPR_MDE0_MASK      0x20
#define VMPR_MDE1_MASK      0x40
#define VMPR_PAGE           (vmpr & VMPR_PAGE_MASK)
#define VMPR_MODE           (vmpr & VMPR_MODE_MASK)
#define VMPR_MODE_3_OR_4    (vmpr & VMPR_MDE1_MASK)

#define BORD_COLOUR_MASK    0x27
#define BORD_KEY_MASK       0x1f
#define BORD_MIC_MASK       0x08
#define BORD_BEEP_MASK      0x10
#define BORD_SPEN_MASK      0x20
#define BORD_EAR_MASK       0x40
#define BORD_SOFF_MASK      0x80
#define BORD_VAL(x)         ((((x) & 0x20 ) >> 2) | ((x) & 0x07))
#define BORD_COL(x)         ((x) & BORD_COLOUR_MASK)
#define BORD_SOFF           (border & BORD_SOFF_MASK)

#define LPEN_TXFMST         0x02    // Bit set if MIDI OUT is currently transmitting a byte


// Bits in the status register to RESET to signal the various interrupts
#define STATUS_INT_LINE     0x01
#define STATUS_INT_MOUSE    0x02    // Part of original SAM design, but never used
#define STATUS_INT_MIDIIN   0x04
#define STATUS_INT_FRAME    0x08
#define STATUS_INT_MIDIOUT  0x10
#define STATUS_INT_NONE     0x1f

// MIDI transfer rates are 31.25 Kbaud. Data has 1 start bit, 8 data bits, and 1 stop bit, for 320us serial byte.
#define MIDI_TRANSMIT_TIME      USECONDS_TO_TSTATES(320)
#define MIDI_INT_ACTIVE_TIME    USECONDS_TO_TSTATES(16)


// Keyboard matrix buffer
extern BYTE keybuffer[9];

// Last port read/written
extern WORD wPortRead, wPortWrite;
extern BYTE bPortInVal, bPortOutVal;

// Paging ports for internal and external memory
extern BYTE vmpr, hmpr, lmpr, lepr, hepr;
extern BYTE vmpr_mode, vmpr_page1, vmpr_page2;

extern BYTE keyboard, border;
extern BYTE border_col;

// Write only ports
extern BYTE line_int;
extern UINT clut[N_CLUT_REGS], mode3clut[4];

// Read only ports
extern BYTE status_reg;
extern BYTE lpen;

extern CDiskDevice *pFloppy1, *pFloppy2, *pBootDrive;
extern CIoDevice *pParallel1, *pParallel2;

extern int g_nAutoLoad;

#endif
