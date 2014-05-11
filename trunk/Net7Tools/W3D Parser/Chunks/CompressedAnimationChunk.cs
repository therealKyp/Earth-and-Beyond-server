using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    public class CompressedAnimationChunk : Chunk
    {
        /// <summary>
        /// <para>WHA (Westwood Hierarchy Animation)</para>
        /// <para>A Hierarchy Animation is a set of data defining deltas from the base 
        /// position of a hierarchy tree. Translation and Rotation channels can be
        /// attached to any node of the hierarchy tree which the animation is 
        /// associated with.</para>
        /// </summary>
        public CompressedAnimationChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|-| " + ChunkHeader.W3D_CHUNK_COMPRESSED_ANIMATION);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_COMPRESSED_ANIMATION;
            HeaderName = ChunkHeader.W3D_CHUNK_COMPRESSED_ANIMATION.ToString();
        }
    }
}
