using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.ComponentModel;

namespace WestWood3D.Chunks
{
    class EmitterInfoChunk : Chunk
    {
        private String texture_name;
        private float start_size;
        private float end_size;
        private float life_time;
        private float emission_rate;
        private float max_emissions;
        private float velocity_random;
        private float position_random;
        private float fade_time;
        private float gravity;
        private float elasticity;
        private Vector3 velocity;
        private Vector3 acceleration;
        private Color start_color;
        private Color end_color;

        public EmitterInfoChunk()
        {
        }

        public override void parse(BinaryReader br, ChunkMap chkMap, Boolean dbg, int chunkLength)
        {
            if (dbg) Console.Out.WriteLine("|--| " + ChunkHeader.W3D_CHUNK_EMITTER_INFO);

            HeaderID = (int)ChunkHeader.W3D_CHUNK_EMITTER_INFO;
            HeaderName = ChunkHeader.W3D_CHUNK_EMITTER_INFO.ToString();

            byte[] tex_name_array = br.ReadBytes(chunkLength - 72);
            texture_name = System.Text.ASCIIEncoding.ASCII.GetString(tex_name_array);
            start_size = br.ReadSingle();
            end_size = br.ReadSingle();
            life_time = br.ReadSingle();
            emission_rate = br.ReadSingle();
            max_emissions = br.ReadSingle();
            velocity_random = br.ReadSingle();
            position_random = br.ReadSingle();
            fade_time = br.ReadSingle();
            gravity = br.ReadSingle();
            elasticity = br.ReadSingle();
            
            velocity = new Vector3();
            velocity.X = br.ReadSingle();
            velocity.Y = br.ReadSingle();
            velocity.Z = br.ReadSingle();
            
            acceleration = new Vector3();
            acceleration.X = br.ReadSingle();
            acceleration.Y = br.ReadSingle();
            acceleration.Z = br.ReadSingle();
            
            start_color = Color.FromArgb(br.ReadByte(), br.ReadByte(), br.ReadByte(), br.ReadByte());
            end_color = Color.FromArgb(br.ReadByte(), br.ReadByte(), br.ReadByte(), br.ReadByte());
            
            if(dbg)
            {
			    Console.Out.WriteLine("\t Texture Name: " + texture_name);
                Console.Out.WriteLine("\t Start Size: " + start_size);
                Console.Out.WriteLine("\t End Size: " + end_size);
                Console.Out.WriteLine("\t Lifetime: " + life_time);
                Console.Out.WriteLine("\t Emission Rate: " + emission_rate);
                Console.Out.WriteLine("\t Max Emissions: " + max_emissions);
                Console.Out.WriteLine("\t Velocity Random: " + velocity_random);
                Console.Out.WriteLine("\t Position Random: " + position_random);
                Console.Out.WriteLine("\t Fade Time: " + fade_time);
                Console.Out.WriteLine("\t Gravity: " + gravity);
                Console.Out.WriteLine("\t Elasticity: " + elasticity);
                Console.Out.WriteLine("\t Velocity: " + velocity);
                Console.Out.WriteLine("\t Acceleration: " + acceleration);
                Console.Out.WriteLine("\t Start Color: " + start_color);
                Console.Out.WriteLine("\t End Color: " + end_color);
            }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The name of the texture to be applied to the particle.")]
        public String TextureName
        {
            get { return texture_name; }
            set { texture_name = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The start size of the emitter.")]
        public float StartSize
        {
            get { return start_size; }
            set { start_size = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The end size of the emitter.")]
        public float EndSize
        {
            get { return end_size; }
            set { end_size = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Lifetime of the emitter.")]
        public float LifeTime
        {
            get { return life_time; }
            set { life_time = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The rate of emissions this particle should produce.")]
        public float EmissionRate
        {
            get { return emission_rate; }
            set { emission_rate = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The maximum number of emissions allowed by this particle.")]
        public float MaxEmissions
        {
            get { return max_emissions; }
            set { max_emissions = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Random Velocity of the particles.")]
        public float VelocityRandom
        {
            get { return velocity_random; }
            set { velocity_random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("Random positions of the particles.")]
        public float PositionRandom
        {
            get { return position_random; }
            set { position_random = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of time it takes for the particle to fad.")]
        public float FadeTime
        {
            get { return fade_time; }
            set { fade_time = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The proposed gravity of the particle.")]
        public float Gravity
        {
            get { return gravity; }
            set { gravity = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The amount of 'stretch' in the particle.")]
        public float Elasticity
        {
            get { return elasticity; }
            set { elasticity = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The velocity of the particles.")]
        public Vector3 Velocity
        {
            get { return velocity; }
            set { velocity = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The acceleration of the particles.")]
        public Vector3 Acceleration
        {
            get { return acceleration; }
            set { acceleration = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The start color of the particles.")]
        public Color StartColor
        {
            get { return start_color; }
            set { start_color = value; }
        }

        [CategoryAttribute("Chunk Values"), ReadOnlyAttribute(true), DescriptionAttribute("The end color of the particles.")]
        public Color EndColor
        {
            get { return end_color; }
            set { end_color = value; }
        }
    }
}
