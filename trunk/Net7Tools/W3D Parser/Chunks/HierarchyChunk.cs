using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    /********************************************************************************

	WHT ( Westwood Hierarchy Tree )

	A hierarchy tree defines a set of coordinate systems which are connected
	hierarchically. The header defines the name, number of pivots, etc. 
	The pivots chunk will contain a W3dPivotStructs for each node in the
	tree. 

	The W3dPivotFixupStruct contains a transform for each MAX coordinate
	system and our version of that same coordinate system (bone). It is 
	needed when the user exports the base pose using "Translation Only".
	These are the matrices which go from the MAX rotated coordinate systems
	to a system which is unrotated in the base pose. These transformations
	are needed when exporting a hierarchy animation with the given hierarchy
	tree file.

	Another explanation of these kludgy "fixup" matrices:

	What are the "fixup" matrices? These are the transforms which
	were applied to the base pose when the user wanted to force the
	base pose to use only matrices with certain properties. For 
	example, if we wanted the base pose to use translations only,
	the fixup transform for each node is a transform which when
	multiplied by the real node's world transform, yeilds a pure
	translation matrix. Fixup matrices are used in the mesh
	exporter since all vertices must be transformed by their inverses
	in order to make things work. They also show up in the animation
	exporter because they are needed to make the animation work with
	the new base pose.

	********************************************************************************/

    public class HierarchyChunk : Chunk
    {
        public HierarchyChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if(dbg) Console.Out.WriteLine("|-| "+ ChunkHeader.W3D_CHUNK_HIERARCHY);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_HIERARCHY;
            HeaderName = ChunkHeader.W3D_CHUNK_HIERARCHY.ToString();
        }
    }
}
