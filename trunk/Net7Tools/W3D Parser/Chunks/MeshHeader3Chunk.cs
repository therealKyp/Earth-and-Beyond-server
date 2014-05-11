using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    public class MeshHeader3Chunk : Chunk
    {
        private int version;
        private int attributes;
        private String mesh_name;
        private String container_name;
        private int num_triangles;
        private int num_vertices;
        private int num_materials;
        private int num_damage_stages;
        private int sort_level;
        private int prelit_version;
        private int future_counts;
        private int vertex_channels;
        private int face_channels;
        private Vector3 bounding_box_minimum;
        private Vector3 bounding_box_maximum;
        private Vector3 bounding_sphere_center;
        private float bounding_sphere_radius;

        public MeshHeader3Chunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int endPosition)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_MESH_HEADER3);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_MESH_HEADER3;
            HeaderName = ChunkHeader.W3D_CHUNK_MESH_HEADER3.ToString();

            version = br.ReadInt32();
            version = version / 62146;
            attributes = br.ReadInt32();
            byte[] meshNameArray = br.ReadBytes(16);
            mesh_name = System.Text.ASCIIEncoding.ASCII.GetString(meshNameArray);
            byte[] containerNameArray = br.ReadBytes(16);
            container_name = System.Text.ASCIIEncoding.ASCII.GetString(containerNameArray);
            num_triangles = br.ReadInt32();
            num_vertices = br.ReadInt32();
            num_materials = br.ReadInt32();
            num_damage_stages = br.ReadInt32();
            sort_level = br.ReadInt32();
            prelit_version = br.ReadInt32();
            future_counts = br.ReadInt32();
            vertex_channels = br.ReadInt32();
            face_channels = br.ReadInt32();

            bounding_box_minimum = new Vector3();
            bounding_box_minimum.X = br.ReadSingle();
            bounding_box_minimum.Y = br.ReadSingle();
            bounding_box_minimum.Z = br.ReadSingle();

            bounding_box_maximum = new Vector3();
            bounding_box_maximum.X = br.ReadSingle();
            bounding_box_maximum.Y = br.ReadSingle();
            bounding_box_maximum.Z = br.ReadSingle();

            bounding_sphere_center = new Vector3();
            bounding_sphere_center.X = br.ReadSingle();
            bounding_sphere_center.Y = br.ReadSingle();
            bounding_sphere_center.Z = br.ReadSingle();

            bounding_sphere_radius = br.ReadSingle();

            if (dbg)
            {
                Console.Out.WriteLine("\t Version: " + version);
                Console.Out.WriteLine("\t Attributes: " + attributes);
                Console.Out.WriteLine("\t Mesh Name: " + mesh_name);
                Console.Out.WriteLine("\t Container Name: " + container_name);
                Console.Out.WriteLine("\t Num Triangles(Polygons): " + num_triangles);
                Console.Out.WriteLine("\t Num Vertices: " + num_vertices);
                Console.Out.WriteLine("\t Num Materials: " + num_materials);
                Console.Out.WriteLine("\t Num Damage Stages: " + num_damage_stages);
                Console.Out.WriteLine("\t Sort Level: " + sort_level);
                Console.Out.WriteLine("\t Prelit Version: " + prelit_version);
                Console.Out.WriteLine("\t Future Counts: " + future_counts);
                Console.Out.WriteLine("\t Vertex Channels: " + vertex_channels);
                Console.Out.WriteLine("\t Face Channels: " + face_channels);
                Console.Out.WriteLine("\t Min Corner of BB: " + bounding_box_minimum);
                Console.Out.WriteLine("\t Max Corner of BB: " + bounding_box_maximum);
                Console.Out.WriteLine("\t Center of Bounding Sphere: " + bounding_sphere_center);
                Console.Out.WriteLine("\t Bounding Sphere Radius: " + bounding_sphere_radius);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The version of this mesh.")]
        public int Version
        {
            get { return version; }
            set { version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Any attributes / flags in this chunk.")]
        public int Attributes
        {
            get { return attributes; }
            set { attributes = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of this mesh.")]
        public String MeshName
        {
            get { return mesh_name; }
            set { mesh_name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The container name of this mesh.")]
        public String ContainerName
        {
            get { return container_name; }
            set { container_name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of triangles in this mesh.")]
        public int TriangleCount
        {
            get { return num_triangles; }
            set { num_triangles = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of vertices in this mesh.")]
        public int VerticesCount
        {
            get { return num_vertices; }
            set { num_vertices = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of materials used in this mesh.")]
        public int MaterialsCount
        {
            get { return num_materials; }
            set { num_materials = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of damage stages in this mesh.")]
        public int DamageStageCount
        {
            get { return num_damage_stages; }
            set { num_damage_stages = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The sort level of the mesh.")]
        public int SortLevel
        {
            get { return sort_level; }
            set { sort_level = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Does this mesh include a prelit version")]
        public int PrelitVersion
        {
            get { return prelit_version; }
            set { prelit_version = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of future counts.")]
        public int FutureCount
        {
            get { return future_counts; }
            set { future_counts = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of vetex channels")]
        public int VertexChannels
        {
            get { return vertex_channels; }
            set { vertex_channels = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of face channels.")]
        public int FaceChannels
        {
            get { return face_channels; }
            set { face_channels = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The minimum position of the bounding box.")]
        public Vector3 BoundingBoxMinimum
        {
            get { return bounding_box_minimum; }
            set { bounding_box_minimum = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The maximum position of the bounding box.")]
        public Vector3 BoundingBoxMaximum
        {
            get { return bounding_box_maximum; }
            set { bounding_box_maximum = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The center position of the bounding sphere.")]
        public Vector3 BoundingSphereCenter
        {
            get { return bounding_sphere_center; }
            set { bounding_sphere_center = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The radius size of the Boudning sphere.")]
        public float BoundingSphereRadius
        {
            get { return bounding_sphere_radius; }
            set { bounding_sphere_radius = value; }
        }
    }
}
