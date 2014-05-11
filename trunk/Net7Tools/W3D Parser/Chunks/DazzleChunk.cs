using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    /********************************************************************************

	    Dazzle Objects

	    The only data needed to instantiate a dazzle object is the type-name of
	    the dazzle to use. The dazzle is always assumed to be at the pivot point
	    of the bone it is attached to (you should enable Export_Transform) for 
	    dazzles. If the dazzle-type (from dazzle.ini) is directional, then the 
	    coordinate-system of the bone will define the direction.

    ********************************************************************************/

    public class DazzleChunk : Chunk
    {
        public DazzleChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|-| " + ChunkHeader.W3D_CHUNK_DAZZLE);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_DAZZLE;
            HeaderName = ChunkHeader.W3D_CHUNK_DAZZLE.ToString();
        }
    }
}
