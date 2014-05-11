using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;

using WestWood3D.Chunks;

namespace WestWood3D
{
    ///<summary>
    ///The base C# class for parsing a westwood 3d (.w3d) file structure.
    ///</summary>
    public class w3d
    {
        private Boolean debugText;
        private ChunkMap chkMap;
        private BinaryReader br;
        private Chunk mainChunk;
        private String _filename;
        private String _path;

        ///<summary>
        ///Class Contructor
        ///</summary>
        public w3d(String path, String fileName, Boolean debug)
        {
            //Initialize variables
            chkMap = new ChunkMap();
            debugText = debug;
            _filename = fileName;
            _path = path;

            //Setup and open file for reading
            String file = path + fileName;
            FileStream fileStream = new FileStream(@file, FileMode.Open);
            br = new BinaryReader(fileStream);

            //Start the parsing process.
            beginChunkChopping();

            //Close Binary Reader & close file
            fileStream.Close();
            br.Close();
        }

        ///<summary>
        ///Overloaded Class Contructor
        ///</summary>
        public w3d(String fileName, Boolean debug)
        {
            //Initialize variables
            chkMap = new ChunkMap();
            debugText = debug;
            _filename = fileName;

            //Setup and open file for reading
            FileStream fileStream = new FileStream(@fileName, FileMode.Open);
            br = new BinaryReader(fileStream);

            //Start the parsing process.
            beginChunkChopping();

            //Close Binary Reader & close file
            fileStream.Close();
            br.Close();
        }

        ///<summary>
        ///Chopper method starts the chunk chopping process and
        ///stores the root chunk in memory for future retreival.
        ///</summary>
        private void beginChunkChopping()
        {
            //Setup main chunk to hold the entire hierarchy
            mainChunk = new RootChunk();

            //Make sure we catch any errors
            try
            {
                // Iterate over the entire file processing the chunks
                while (br.BaseStream.Position < br.BaseStream.Length)
                {
                    this.processChunks(mainChunk);
                }
            }
            catch (Exception e)
            {
                //TODO: Add some exeption protection.
                Console.Out.WriteLine(e);
            }
        }

        ///<summary>
        ///Iterates over the complete file, parses the files information
        ///and stores it in individual chunk classes.
        ///</summary>
        private void processChunks(Chunk Parent)
        {
            //Parse Headers and setup local variables
            int chunkID = br.ReadInt32();
            int currentChunkLength = br.ReadInt32() & 0x7FFFFFFF;
            int endLength = (int) br.BaseStream.Position + currentChunkLength;

            // Lookup and create the new chunk class
            Chunk chk = chkMap.getChunk(chunkID);

            //If chunk class exists parse it, if not skip it.
            if (chk != null)
            {
                // Parse the newly created chunk.
                chk.parse(br, chkMap, debugText, currentChunkLength);
                Parent.addSubChunk(chk);
            }
            else
            {
                if(debugText) Console.Out.WriteLine("File: " + _filename + "\t : \tSkipping Chunk: \t " + chunkID);

                //Chunk will be skipped if its not initialized in the chunkmap
                br.BaseStream.Seek(currentChunkLength, SeekOrigin.Current); //Advance to the next chunk
            }

            // Parse any sub chunks
            while (br.BaseStream.Position < endLength)
            {
                processChunks(chk); 
            }
        } // End of processMainChunks

        ///<summary>
        ///Retrievs the main root chunk which contains the entire w3d hierarchy.
        ///</summary>
        public Chunk getRootChunk()
        {
            return mainChunk;
        }
    }
}
