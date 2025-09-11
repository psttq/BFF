import hashlib
import os
import random
import string

def generate_random_md5():
    # Generate random data
    random_data = os.urandom(32)  # 32 bytes of random data
    # Create MD5 hash
    md5_hash = hashlib.md5(random_data).hexdigest()
    return md5_hash

virus_names =  [
    "ILOVEYOU",
    "Melissa",
    "Code Red",
    "Nimda",
    "SQL Slammer",
    "Blaster",
    "Sasser",
    "Conficker",
    "Stuxnet",
    "CryptoLocker",
    "WannaCry",
    "Petya/NotPetya",
    "Zeus",
    "Storm Worm",
    "MyDoom",
    "Sobig",
    "Klez",
    "Anna Kournikova",
    "Chernobyl (CIH)",
    "Michelangelo",
    "Morris Worm",
    "Brain",
    "Jerusalem",
    "Cascade",
    "Form",
    "TeaTimer",
    "Elk Cloner",
    "Concept",
    "ExploreZip",
    "Happy99"
]

lines_number = 1000000

file = open("base.csv", "w+")

lines = []

for i in range(lines_number):
    virus_index = random.randint(0, len(virus_names)-1)
    virus = virus_names[virus_index]
    line = generate_random_md5() + ";" + virus + "\n"
    lines.append(line)
file.writelines(lines)
