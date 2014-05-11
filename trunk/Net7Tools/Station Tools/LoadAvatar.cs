using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;

namespace Station_Tools
{
    class LoadAvatar
    {
        public int avatarType;
        public byte avaterVersion;
        public int race;
        public int profession;
        public int gender;
        public int moodType;
        public byte personality;
        public byte nlp;
        public byte bodyType;
        public byte pantsType;
        public byte headType;
        public byte hairNum;
        public byte earNum;
        public byte goggleNum;
        public byte beardNum;
        public byte weaponHipNum;
        public byte weaponUniqueNum;
        public byte weaponBackNum;
        public byte headTextureNum;
        public byte tattooTextureNum;
        public float[] tattooOffset = new float[3];
        public float[] hairColor = new float[3];
        public float[] beardColor = new float[3];
        public float[] eyeColor = new float[3];
        public float[] skinColor = new float[3];
        public float[] shirtPrimaryColor = new float[3];
        public float[] shirtSecondaryColor = new float[3];
        public float[] pantsPrimaryColor = new float[3];
        public float[] pantsSecondaryColor = new float[3];
        public int shirtPrimaryMetal;
        public int shirtSecondarymetal;
        public int pantsPrimaryMetal;
        public int pantsSecondaryMetal;
        public float[] bodyWeight = new float[5];
        public float[] headWeight = new float[5];

        public LoadAvatar(Stream File)
        {
            BinaryReader bw = new BinaryReader(File);
           
            bw.BaseStream.Position = 44;        // Read avatar data

            avatarType = bw.ReadInt32();        // Read in 4 Byte
            // Read Char
            avaterVersion = bw.ReadByte();
            // Read 32 Bytes
            race = bw.ReadInt32();
            profession = bw.ReadInt32();
            gender = bw.ReadInt32();
            moodType = bw.ReadInt32();
            // All Bytes
            personality = bw.ReadByte();
            nlp = bw.ReadByte();
            bodyType = bw.ReadByte();
            pantsType = bw.ReadByte();
            headType = bw.ReadByte();
            hairNum = bw.ReadByte();
            earNum = bw.ReadByte();
            goggleNum = bw.ReadByte();
            beardNum = bw.ReadByte();
            weaponHipNum = bw.ReadByte();
            weaponUniqueNum = bw.ReadByte();
            weaponBackNum = bw.ReadByte();
            headTextureNum = bw.ReadByte();
            tattooTextureNum = bw.ReadByte();


            // Arrays
            tattooOffset[0] = bw.ReadSingle();
            tattooOffset[1] = bw.ReadSingle();
            tattooOffset[2] = bw.ReadSingle();
            
            hairColor[0] = bw.ReadSingle();
            hairColor[1] = bw.ReadSingle();
            hairColor[2] = bw.ReadSingle();

            beardColor[0] = bw.ReadSingle();
            beardColor[1] = bw.ReadSingle();
            beardColor[2] = bw.ReadSingle();

            eyeColor[0] = bw.ReadSingle();
            eyeColor[1] = bw.ReadSingle();
            eyeColor[2] = bw.ReadSingle();

            skinColor[0] = bw.ReadSingle();
            skinColor[1] = bw.ReadSingle();
            skinColor[2] = bw.ReadSingle();

            shirtPrimaryColor[0] = bw.ReadSingle();
            shirtPrimaryColor[1] = bw.ReadSingle();
            shirtPrimaryColor[2] = bw.ReadSingle();

            shirtSecondaryColor[0] = bw.ReadSingle();
            shirtSecondaryColor[1] = bw.ReadSingle();
            shirtSecondaryColor[2] = bw.ReadSingle();

            pantsPrimaryColor[0] = bw.ReadSingle();
            pantsPrimaryColor[1] = bw.ReadSingle();
            pantsPrimaryColor[2] = bw.ReadSingle();

            pantsSecondaryColor[0] = bw.ReadSingle();
            pantsSecondaryColor[1] = bw.ReadSingle();
            pantsSecondaryColor[2] = bw.ReadSingle();

            shirtPrimaryMetal = bw.ReadInt32();
            shirtSecondarymetal = bw.ReadInt32();
            pantsPrimaryMetal = bw.ReadInt32();
            pantsSecondaryMetal = bw.ReadInt32();

            bodyWeight[0] = bw.ReadSingle();
            bodyWeight[1] = bw.ReadSingle();
            bodyWeight[2] = bw.ReadSingle();
            bodyWeight[3] = bw.ReadSingle();
            bodyWeight[4] = bw.ReadSingle();

            headWeight[0] = bw.ReadSingle();
            headWeight[1] = bw.ReadSingle();
            headWeight[2] = bw.ReadSingle();
            headWeight[3] = bw.ReadSingle();
            headWeight[4] = bw.ReadSingle();

            /* Debug Information */
            /*
		    Console.Out.WriteLine(avatarType);
		    Console.Out.WriteLine(avaterVersion);
		    Console.Out.WriteLine(race);
		    Console.Out.WriteLine(profession);
		    Console.Out.WriteLine(gender);
		    Console.Out.WriteLine(moodType);
		    Console.Out.WriteLine(personality);
		    Console.Out.WriteLine(nlp);
		    Console.Out.WriteLine(bodyType);
		    Console.Out.WriteLine(pantsType);
		    Console.Out.WriteLine(headType);
		    Console.Out.WriteLine("hairNum: "+hairNum);
		    Console.Out.WriteLine(earNum);
		    Console.Out.WriteLine(goggleNum);
		    Console.Out.WriteLine(beardNum);
		    Console.Out.WriteLine(weaponHipNum);
		    Console.Out.WriteLine(weaponUniqueNum);
		    Console.Out.WriteLine(weaponBackNum);
		    Console.Out.WriteLine(headTextureNum);
		    Console.Out.WriteLine(tattooTextureNum);
		    Console.Out.WriteLine(tattooOffset[0]+","+tattooOffset[1]+","+tattooOffset[2]);  
		    Console.Out.WriteLine(hairColor[0]+","+hairColor[1]+","+hairColor[2]);  
		    Console.Out.WriteLine(beardColor[0]+","+beardColor[1]+","+beardColor[2]);  
		    Console.Out.WriteLine(eyeColor[0]+","+eyeColor[1]+","+eyeColor[2]);  
		    Console.Out.WriteLine(skinColor[0]+","+skinColor[1]+","+skinColor[2]);  
		    Console.Out.WriteLine(shirtPrimaryColor[0]+","+shirtPrimaryColor[1]+","+shirtPrimaryColor[2]); 
		    Console.Out.WriteLine(shirtSecondaryColor[0]+","+shirtSecondaryColor[1]+","+shirtSecondaryColor[2]); 
		    Console.Out.WriteLine(pantsPrimaryColor[0]+","+pantsPrimaryColor[1]+","+pantsPrimaryColor[2]); 
		    Console.Out.WriteLine(pantsSecondaryColor[0]+","+pantsSecondaryColor[1]+","+pantsSecondaryColor[2]); 
		    Console.Out.WriteLine(shirtPrimaryMetal);
		    Console.Out.WriteLine(shirtSecondarymetal);
		    Console.Out.WriteLine(pantsPrimaryMetal);
		    Console.Out.WriteLine(pantsSecondaryMetal);
		    Console.Out.WriteLine(bodyWeight[0]+","+bodyWeight[1]+","+bodyWeight[2]+","+bodyWeight[3]+","+bodyWeight[4]); 
		    Console.Out.WriteLine(headWeight[0]+","+headWeight[1]+","+headWeight[2]+","+headWeight[1]+","+headWeight[2]);
            */

        }
    }
}
