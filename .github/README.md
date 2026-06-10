# ps-patch-system

App patching system for PS4/PS5.

![](https://github.com/user-attachments/assets/ccfc5a08-711c-4c7b-8aa7-b38c5a24c095)

- (Yes, I really commissioned an artist for this)
  - [Makise Kurisu](https://scienceadventure.wiki.gg/wiki/Makise_Kurisu) from [Steins;Gate](https://scienceadventure.wiki.gg/wiki/Steins;Gate_(visual_novel))
  - [PS5 Model](https://sketchfab.com/3d-models/ps5-d788de3735964151a3e24fd59c0f1956) from rtql8d at Sketchfab
  - ```
    "PS5" (https://sketchfab.com/3d-models/ps5-d788de3735964151a3e24fd59c0f1956) by rtql8d
    is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
    ```
<details><summary>

### Screenshots (PS4)
</summary>

![](https://github.com/user-attachments/assets/7eab0451-6672-4e63-821e-74744377b6cf)
![](https://github.com/user-attachments/assets/6071ae11-8e64-438f-9ce3-7561314723cb)
![](https://github.com/user-attachments/assets/0cfa1d8c-a6b7-4a0c-b593-42d508eba79c)
![](https://github.com/user-attachments/assets/ab813ae4-75fa-45a6-9580-6bab2b2d5b62)

</details>

<details><summary>

### Screenshots (PS5)
</summary>

</details>

# Usage

- Start elfldr [(PS4)](https://github.com/ps4-payload-dev/elfldr) [(PS5)](https://github.com/ps5-payload-dev/elfldr), will listen for payload at port 9021
- Send [patch-bundle-loader-(platform).elf](../../../releases/latest) to port 9021
- Extract and upload [patch1.zip](https://github.com/illusionyy/PS-Game-Patch/releases/latest) xml files to `/data/user/patches`
  - `xml_orbis` for PS4 Patches
  - `xml_prospero` for PS5 Patches
- Open webpage at `127.0.0.1:23900` or console IP on another device.
- Apply patches and they should work!

### Confirmed working firmwares:
- 3.00 - 11.70

### Looking for 1080p limit, direct memory and native app clock patch?

Checkout branch `df-demo-550`. only for 5.50 PS5 retail, nothing else!

# Credits

- Anonymous Artist for Kurisu Render
- [John Törnblom](https://github.com/john-tornblom)
  - [PS4](https://github.com/ps4-payload-dev/sdk) and [PS5](https://github.com/ps5-payload-dev/sdk) Payload SDK
  - [`sigqueue`](shared/sigqueue.c) implementation
  - Signaling idea between processes
- [idlesauce](https://github.com/idlesauce)
  - [`proc_rw`](shared/proc_rw.c) memory write trick used in firmware 8.20+ on PS5.
- [fast-lzma2 + contributors](https://github.com/conor42/fast-lzma2) compression library
