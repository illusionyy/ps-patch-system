# ps-patch-system

App patching system for PS4/PS5.

![](https://github.com/user-attachments/assets/fd047e71-391e-46d2-91a8-5bde99dfdf7d)

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

![](https://github.com/user-attachments/assets/cacae4f4-04dc-4286-9b64-a3d013fbb9c7)
![](https://github.com/user-attachments/assets/4dd1c6c3-2f3d-4390-97cf-38b9e2ed312c)
![](https://github.com/user-attachments/assets/e817bc60-ea8e-40b2-8b32-241ee8a42df4)
![](https://github.com/user-attachments/assets/2770438b-0403-4346-a2f5-445fd358f42a)

</details>

<details><summary>

### Screenshots (PS5)
</summary>

![](https://github.com/user-attachments/assets/7edc457b-12b8-4224-b12c-fcaeab7a8dcc)
![](https://github.com/user-attachments/assets/ad3e631a-4757-448b-8541-7980cd7f8b7d)
![](https://github.com/user-attachments/assets/ee307fb9-1a85-4580-bf21-c21b6d323a45)
![](https://github.com/user-attachments/assets/69f5cdb6-bfa3-4c2d-b486-2fbcf84585d9)

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
