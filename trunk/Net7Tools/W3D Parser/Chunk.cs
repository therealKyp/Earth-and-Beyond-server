using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.IO;
using System.ComponentModel;

namespace WestWood3D
{
    public abstract class Chunk
    {
        private Hashtable subChunks = new Hashtable();
        private int header_id;
        private String header_name;

        ///<summary>
        ///No arg Constructor
        ///</summary>
        public Chunk()
        {
        }

        ///<summary>
        ///Adds sub chunks to this chunk.
        ///</summary>
        public void addSubChunk(Chunk subChunk)
        {
            subChunks.Add(subChunks.Count, subChunk);
        }

        public abstract void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition);

        ///<summary>
        ///Retrieves the sub chunks of this chunk.
        ///</summary>
        public Hashtable getSubChunks()
        {
            return subChunks;
        }

        [CategoryAttribute("Class Properties"), ReadOnlyAttribute(true), DescriptionAttribute("The Header ID for the chunk.")]
        public int HeaderID
        {
            get { return header_id; }
            set { header_id = value; }
        }

        [CategoryAttribute("Class Properties"), ReadOnlyAttribute(true), DescriptionAttribute("The Header Name for the chunk.")]
        public String HeaderName
        {
            get { return header_name; }
            set { header_name = value; }
        }
    }
}
