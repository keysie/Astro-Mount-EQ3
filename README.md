# Astro-Mount-EQ3 #

## Description ##

This project is a DIY improvement for a cheap EQ3-like equatorial telescope mount. Cheap mounts are often sold with motor packages that are supposed to automatically turn the hour axis and thus compensate for the "movement" of stars due to earth's rotation. As they are cheap, the do not include any accurate timing or feedback control, thus making it impossible to use the mount for even the most basic astrophotography. This project tackles this problem by using a control system based on an Arduino Uno R3, a cheap dc-motor with gear, and a self-made magnetic encoder board.

## Verifying code and author ##

My (keysie) public gpg key is stored within this repository as a raw blob. It is tagged by tag *gpg-public*. To add it to your gpg keyring, use

`git cat-file blob gpg-public | gpg --import`
