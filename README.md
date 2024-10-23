This is a small role playing game, where characters fight each other
Each character has the following data:
- name is the name of the character, which is a string that can have arbitrary length.
* hit points (HP) is the health level of the character, which is a positive integer. HP is reduced when the character gets damage in the fight. The character will die when HP reduces to zero.
* experience is the experience level of the character, which is a non-negative integer. It increases when the character hits its opponents in the fight. The experience points increase by the amount of inflicted damage. A new character starts with zero experience.
* weapon that the character carries. A weapon is defined with the following data.
  * name is the name of the weapon.
  * damage is the damage the weapon may inflict.

The game allows players to use the following commands:
1. **Add Character Command**  
   Adds a new character to the game.* Attack command, which makes the given character to attack a target character: A <name> <hit-points> <weapon-name> <weapon-damage>
   - `<name>`: The name of the character to add.
   - `<hit-points>`: The health points the character has.
   - `<weapon-name>`: The name of the weapon the character will use.
   - `<weapon-damage>`: The damage value of the character's weapon.

2. **Attack Command**  
Makes the given character attack a target character:  H \<attacker-name\> \<target-name\>
   - `<attacker-name>`: The character initiating the attack.
   - `<target-name>`: The target character being attacked.

3. **Print Game Command**  
Displays the game content: L
 
4. **Save to File Command**
Saves the database to a text file: W \<filename\>
   - `<filename>`: The name of the file where the game data will be saved.

5. **Load from File Command**
Loads data associated with an existing game from a text file saved using the Save to File command: O \<filename\>
   - `<filename>`: The name of the file from which to load game data.

6. **Quit program command**
   Releases all allocated memory and exits the program: Q

