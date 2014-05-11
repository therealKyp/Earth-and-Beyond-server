using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;

using WestWood3D.Chunks;

namespace WestWood3D
{
    class ChunkChopper
    {
        private ChunkMap _chkMap;
        private BinaryReader _bw;
        private Boolean _debug;
        private Chunk mainChunk;
        private String _filename;

        ///<summary>
        ///ChunkChopper Constructer that starts the chunk chopping process and
        ///stores the root chunk in memory for future retreival.
        ///</summary>
        public ChunkChopper(BinaryReader bw, Boolean debugText, String filename)
        {
            _chkMap = new ChunkMap();
            _bw = bw;
            _debug = debugText;
            _filename = filename;

            mainChunk = new RootChunk();
            while (_bw.BaseStream.Position < _bw.BaseStream.Length)
            {
                this.processMainChunks(mainChunk);
            }
            
            //printChunkHeirarchy(mainChunk, 0);
        }

        ///<summary>
        ///Iterates over the complete file, parses the files information
        ///and stores it in individual chunk classes.
        ///</summary>
        private void processMainChunks(Chunk Parent)
        {
            //Parse Headers and setup local variables
            int chunkID = _bw.ReadInt32();
            int currentChunkLength = _bw.ReadInt32() & 0x7FFFFFFF;
            int endLength = (int) _bw.BaseStream.Position + currentChunkLength; 

            // Lookup and create the new chunk class
            Chunk chk = _chkMap.getChunk(chunkID);

            //If chunk class exists parse it, if not skip it.
            if (chk != null)
            {
                // Parse the newly created chunk.
                chk.parse(_bw, _chkMap, _debug, currentChunkLength);
                Parent.addSubChunk(chk);
            }
            else
            {
                //if(_debug) 
                    Console.Out.WriteLine("File: " + _filename + "\t : \tSkipping Chunk: \t " + chunkID);

                //Chunk will be skipped if its not initialized in the chunkmap
                _bw.BaseStream.Seek(currentChunkLength, SeekOrigin.Current); //Advance to the next chunk
            }

            while (_bw.BaseStream.Position < endLength)
            {
                processMainChunks(chk); 
            }
        } // End of processMainChunks

        public void printChunkHeirarchy(Chunk chunk, int level)
        {
            int _level = level;
            Hashtable tmp = chunk.getSubChunks();
            IDictionaryEnumerator Enumerator = tmp.GetEnumerator();

            while (Enumerator.MoveNext())
            {
                Chunk tmpChunk = (Chunk) Enumerator.Value;
                String tab = "";
                switch (_level)
                {
                    case 0:
                        tab = "";
                        break;
                    case 1:
                        tab = "\t";
                        break;
                    case 2:
                        tab = "\t\t";
                        break;
                    case 3:
                        tab = "\t\t\t";
                        break;
                }
                Console.Out.WriteLine(tab + Enumerator.Value.GetType() + ", Level: " + _level);
                printChunkHeirarchy(tmpChunk, _level + 1);
            }

            //return mainChunk;
        }
    }
}
