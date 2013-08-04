/*
From 121055711617@post2.tele.dkTue Aug  6 11:26:02 1996
Date: Tue, 06 Aug 96 12:59:42 CET
From: Jakob Havkrog <121055711617@post2.tele.dk>
Reply to: Jacob Havkrog Joergensen <havkrog@post2.tele.dk>
To: emarch@eagle.ais.net
Subject: [Fwd: [Fwd: Re: NumLock. How do I set it with PM from BC+

For a PM app, you may skip the KBD$ part (i.e., just use the
WinSetKeyboardStateTable part).
-----------------------------------------------------------------------
*/

#define INCL_DOSFILEMGR
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_WININPUT

#include <os2.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    HFILE hf;
    SHIFTSTATE ss;
    BYTE KeyState[257];
    BOOL bFlag;
    APIRET rc;

    ULONG ulAction, ulLength;

    rc = DosOpen("KBD$",
               &hf,
               &ulAction,
               0L,
               0,
               FILE_OPEN,
               OPEN_ACCESS_READONLY|OPEN_SHARE_DENYNONE,
               0);

    ulAction = 0;
    ulLength = sizeof(ss);
    rc = DosDevIOCtl(hf,
                   IOCTL_KEYBOARD,
                   KBD_GETSHIFTSTATE,
                   0,
                   0,
                   &ulAction,
                   &ss,
                   sizeof(ss),
                   &ulLength);

    WinSetKeyboardStateTable(HWND_DESKTOP, KeyState, FALSE);

    int bSet = 0;
    if(argc < 2)
    {
        printf("*** FreeWare! ***\n\n");
        printf("\r\nUsage:   NUMLOCK  S[et] | R[eset]\n");
        printf("Examples:     NUMLOCK S\n");
        printf("Examples:     NUMLOCK Set\n");
        printf("Examples:     NUMLOCK R\n");
        printf("Examples:     NUMLOCK Reset\n");
        printf("May be added to the config.sys as 'RUN=NUMLOCK Set'\n");
        return(0);
    }

    if(argc > 1 && (argv[1][0] == 'S' || argv[1][0] == 's') )
    {
        bSet = 1;
    }

    /* if bSet is true, enable NUMLOCK */
    if(bSet)
    {
        KeyState[VK_NUMLOCK] |= 0x01;
        ss.fsState |= NUMLOCK_ON;
    }
    else
    {
        KeyState[VK_NUMLOCK] &= ~0x01;
        ss.fsState &= ~NUMLOCK_ON;
    }

    WinSetKeyboardStateTable(HWND_DESKTOP, KeyState, TRUE);

    ulAction = sizeof(ss);
    ulLength = 0;
    rc = DosDevIOCtl(hf,
                   IOCTL_KEYBOARD,
                   KBD_SETSHIFTSTATE,
                   &ss,
                   sizeof(ss),
                   &ulAction,
                   0,
                   0,
                   &ulLength);

    rc = DosClose(hf);
}
