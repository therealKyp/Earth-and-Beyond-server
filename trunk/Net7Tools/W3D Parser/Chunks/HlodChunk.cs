using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    /********************************************************************************

	HLod (Hierarchical LOD Model)

	This is a hierarchical model which has multiple arrays of models which can
	be switched for LOD purposes.

	Relevant Chunks:
	----------------
	W3D_CHUNK_HLOD										=0x00000700,		// description of an HLod object (see HLodClass)
		W3D_CHUNK_HLOD_HEADER,												// general information such as name and version
		W3D_CHUNK_HLOD_LOD_ARRAY,											// wrapper around the array of objects for each level of detail
			W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER,					// info on the objects in this level of detail array
			W3D_CHUNK_HLOD_SUB_OBJECT,										// an object in this level of detail array
		W3D_CHUNK_HLOD_AGGREGATE_ARRAY,									// array of aggregates, contains W3D_CHUNK_SUB_OBJECT_ARRAY_HEADER and W3D_CHUNK_SUB_OBJECT_ARRAY
		W3D_CHUNK_HLOD_PROXY_ARRAY,										// array of proxies, used for application-defined purposes

	An HLOD is the basic hierarchical model format used by W3D. It references
	an HTree for its hierarchical structure and animation data and several arrays
	of sub-objects; one for each LOD in the model. In addition, it can contain
	an array of "aggregates" which are references to external W3D objects to
	be automatically attached into it. And it can have a list of "proxy" objects
	which can be used for application purposes such as instantiating game objects
	at the specified transform. 

	********************************************************************************/

    public class HlodChunk : Chunk
    {
        /// <summary>
        /// This is a hierarchical model which has multiple arrays of models which can be switched for LOD purposes.
        /// </summary>
        public HlodChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|-| " + ChunkHeader.W3D_CHUNK_HLOD);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HLOD;
            HeaderName = ChunkHeader.W3D_CHUNK_HLOD.ToString();
        }
    }
}
