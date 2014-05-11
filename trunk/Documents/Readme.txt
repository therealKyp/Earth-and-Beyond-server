Net7 readme.txt

------------------------
Created October 25, 2006
------------------------

Compiling the C++ source code requires Microsoft Visual C++ 6.0.  You may be able to compile it with Microsoft Visual Studio 2003 or 2005; I have not tried.

The contents of the entire project trunk should be copied to the C:\Net7\ folder.  I recommend using synchronization tools such as WinDiff (Microsoft) or SynchronizeIt! and CompareIt! by Grig Software (grigsoft.com) to synchronize files between the Net7 folder and the enbemu_repos folder.

The programs in Net7 were put together in a hurry, and are a bit sloppy.  Many of the paths are hard-coded.  For instance, the logging path is hard-coded to go to the C:\Net7\logs\ folder.  The Launcher, ClientDetours, and ClientUpdate programs are hard-coded to look for Earth and Beyond in the following location:

	"C:\Program Files\EA GAMES\Earth & Beyond"

To use ClientDetours.dll, simply run Launcher.exe.

The Net7 application is not quite ready to be released.  Currently, all network traffic is directed to "auth.enb-emulator.net" and uses the hosts file to redirect it to 127.0.0.1.  The bin folder contains a self-signed certificate for using SSL with the auth.enb-emulator.net domain.

One final note:

If you are not an expert with these tools, don’t bother asking for help compiling the software.  The developers have their hands full trying to handle their full time jobs while continuing development on the emulator.

VectoR.360

-----------------------
Update October 31, 2006
-----------------------

Here is the first public release of the Net7 server.
Run the Net7.exe application in the bin folder with no command line parameters to use it as a standalone server on localhost.

Source code is included.  Don't bother to tell me the code is crappy, I already know.  This is pre-alpha, and I put it together in about three weeks in my  spare time.  It is just a starting point.  The program does NOT work when built in Release mode, so this build is in Debug mode.  Fixing this is not at the top of my priority list.

Here is what you CAN presently do with this server:

    - Edit the 'accounts.txt' file to add your username and password (I recommend 'go' for quick entry).
    - Character selection: You can delete toons, create toons, select toons.
      Note that toon files are saved in the database folder.  The server does NOT use a database.
    - start out in Earth Station
    - run around the station (works well)
    - interact with the NPCs (barely)
    - interact with static players (well, not really)
    - interact with terminals (although you can't DO anything but view it)
    - exit the station into Earth Sector
    - in space, you can select objects, even yourself
    - if you select Earth Station, the Dock and Register verbs are presented
    - if you click the Dock button, you are immediately docked back in Earth Station (no docking camera)


Here are the things you CANNOT presently do;

    - MULTI-PLAYER is NOT yet supported
    - No other sectors or stations are present
    - Distributed Sector Server support is NOT complete
    - You cannot move around in space (no impulse engines)
    - You cannot WARP, so you are stuck right next to Earth Station
    - Chat is not yet supported
    - Cannot do anything at the terminals, other than view them
    - If you try to interact with the Avatar Customization or Ship Customization,
      you WILL get stuck.  The only way out is '/logout'.
    - No private chat rooms
    - No NPC interaction
    - No MOBS
    - No prospecting
    - No missions
    - No weapons
    - No inventory
    - No mining
    - No trading, buying, selling
    - No groups
    - No guilds
    - No formations
    - Remaining three character types not available
    - Anything else not listed above


Note that Net7 uses a different domain name than the Java version.  The domain name is set to 'auth.enb-emulator.net' which is redirected to localhost (127.0.0.1) via the hosts file.

There are a number of setup steps that are required to use the Net7 server.  To make it easy, I automated these steps using a Client Update application.

The Client Update application has been developed to do the following:

    - Patch the URL in the rg_regdata.ini file
    - Patch the URL in the auth.ini file
    - If running a local Server, add auth.enb-emulator.net to the hosts file.
    - Patch client.exe to remove the EULA and Rules of Conduct dialogs
    - Update the registry to mark the client as “Registered”
    - Launch a browser window to take the user to this URL
      o  https://auth.enb-emulator.net/NewPlayer.html
    - Launch the client with the appropriate command line switches

Note that all of these steps are OPTIONAL.

I apologize to those of you who do not have E&B installed in the standard location on drive C.  The Client Updater is hard coded to expect the program to be in the standard location. (Sorry, Blasti99).

In order to proceed, the user must accept/import the certificate.
Run the launcher application to launch E&B.

As time permits, I will develop a better Launcher.  The launcher in the
bin folder is designed specifically for attaching the ClientDetours.dll
to the client application.

I know this is half-baked at this point.  I am releasing what I have now so
the community can get a taste of the pleasure I have had in the last couple
of weeks.

Note to all: Feel free to modify the code and submit changes to SVN.
Note to agfot: Feel free to work on adding Movement to the Net7 version :)

Enjoy!
VectoR.360
email: VectoR.360@gmail.com

------------------------
Update November 12, 2006
------------------------

This release includes a number of bug fixes, as well as a few new features.

1. The hulls, wings, profession, and engines displayed should now match what you choose when creating your character.

2. The main feature that was added is that the Net-7 Server now allows you to dock at three station types: Terran, Jenquai, and Progen.  When you create a new character, the character will start out in the correct station.

3. If your client is not up to date (version 42.0), you will get a notice that you must update the client.  Run enb-up.exe to update the client to the latest version.

4. The Galaxy Map is now working.

5. When you launch into space, the your engines, shields, reactor, devices, and weapons are now properly installed.

6. The launcher has been modified to allow you to connect to a multi-player server.  Certificate installation has been streamlined somewhat.  (The only real solution is to buy a certificate from a registered certificate authority).

7. The Net-7 Server has been restructured somewhat to provide support for multi-player capability. This is not quite ready for prime time, though.

8. Added "Net-7 FAQ.txt" by Jadefalcon.  If you have problems, start there before posting on the forum.

VectoR.360

------------------------
Update December 22, 2006
------------------------

It has been quite a while since the last release.  This release has a LOT of new features.

NOTICE: You must install the local.net-7.org certificate in order to use this release.
Double-click local.net-7.org.cer in the C:\Net7\bin folder to install the certificate.

NOTICE: The player files have been moved from the C:\Net7\database\ folder to the
C:\Net7\database\Users\ folder.  If you want to keep your existing player files, be
sure to move them to the new location.

NOTICE: Multi-player is UNSUPPORTED.  Do not ask how to get multiplayer working on the forum!
Multi-player capability is partially working, but still in development.

1. Multiplayer capabilties have been added:
   You can now see other players in the station.
   You can now see other players in space.
   You can now chat with other players.  Note that private /tell messages are not supported.

2. Patched a few client files.
   The player may now use the in-game web browser i.e. "Intergalactic Net" and Knowledgebase
   Updated EB_Sizzle.bik with Net-7 logo

3. Movement has been added, allowing players to move around in space.  Movement is still
   a bit primitive and not very smooth, but it is much better than the last release!
   Warp is not functional yet.

4. Updated the Net-7 Launcher to enhance support for the multi-player server.

5. Sector content is now dynamic. Instead of being stuck in Earth Sector, you now have
   access to all Sol sectors.

6. Gates are now functional.  You can travel between sectors in Sol.  Gates leading out
   of Sol are "under construction".  You must be within a reasonable range of the gate in
   order to use it.

7. The starting skills are now set correctly for all nine classes.

8. The ranks are now set correctly for all nine classes.

9. You must now be within 5K of a station in order to dock.

10.You can now move items around in your inventory.  You might even be able to trade items
   with other players.

