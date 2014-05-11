using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class NodeChunk : Chunk
    {
        private String render_object_name;
        private int pivot_id;

        public NodeChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_NODE);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_NODE;
            HeaderName = ChunkHeader.W3D_CHUNK_NODE.ToString();

            byte[] renderObjNameArray = br.ReadBytes(chunkLength - 2);
            render_object_name = System.Text.ASCIIEncoding.ASCII.GetString(renderObjNameArray);
            pivot_id = br.ReadInt16();

            if (dbg)
            {
                Console.Out.WriteLine("\t Render Object Name: " + render_object_name);
                Console.Out.WriteLine("\t Pivot Id: " + pivot_id);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the Render Object.")]
        public String RenderObjectName
        {
            get { return render_object_name; }
            set { render_object_name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The pivot id to attach the render object too.")]
        public int PivotID
        {
            get { return pivot_id; }
            set { pivot_id = value; }
        }
    }
}
