using System;
using System.IO;

namespace O3DConverter
{

    class D3DXVECTOR3 
    {
        public float x,y,z;
        public D3DXVECTOR3(BinaryReader binaryReader)
        {
            this.x = binaryReader.ReadSingle();
            this.y = binaryReader.ReadSingle();
            this.z = binaryReader.ReadSingle();
        }
        public static D3DXVECTOR3[] ReadBulk (BinaryReader binaryReader,int count)
        {
            var bulk = new D3DXVECTOR3[count];
            for (int i = 0; i < count; i++)
                bulk[i] = new D3DXVECTOR3(binaryReader);
            return bulk;
        }

    }
    class D3DXMATRIX
    {
        public float _11, _12, _13, _14;
        public float _21, _22, _23, _24;
        public float _31, _32, _33, _34;
        public float _41, _42, _43, _44;

        /// <summary>
        /// Returns Column X of Row Y from Matrix
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        public float this[int x, int y]
        {
            get
            {
                if (x >= 0 && y >= 0 && x < 5 && y < 5)
                    return (int)GetType().GetField("_" + x + y).GetValue(this);
                else
                    return 0;
            }
            set
            {
                if (x > 0 && y > 0 && x < 5 && y < 5) {
                    var prop = GetType().GetField("_" + x + y);
                    prop.SetValue(this, value);
                }
            }
        }

        /// <summary>
        /// Parse Matrix from Binary Reader
        /// </summary>
        /// <param name="binaryReader"></param>
        public D3DXMATRIX(BinaryReader binaryReader)
        {
            for (int y = 1; y < 5; y++)
                for (int x = 1; x < 5; x++)
                    this[x, y] = binaryReader.ReadSingle();
        }
        public static D3DXMATRIX[] ReadBulk(BinaryReader binaryReader,int count)
        {
            var matrices = new D3DXMATRIX[count];
            for (int i = 0; i < count; i++)
                matrices[i] = new D3DXMATRIX(binaryReader);
            return matrices;
        }
    };

    class D3DXQUATERNION
    {
        float x;
        float y;
        float z;
        float w;
        public D3DXQUATERNION(BinaryReader binaryReader)
        {
            x = binaryReader.ReadSingle();
            y = binaryReader.ReadSingle();
            z = binaryReader.ReadSingle();
            w = binaryReader.ReadSingle();
        }
    }
    class D3DCOLORVALUE
    {
        public float r;
        public float g;
        public float b;
        public float a;
        public D3DCOLORVALUE(BinaryReader binaryReader)
        {
            r = binaryReader.ReadSingle();
            g = binaryReader.ReadSingle();
            b = binaryReader.ReadSingle();
            a = binaryReader.ReadSingle();
        }
    }
    class D3DMATERIAL9
    {
        public D3DCOLORVALUE Diffuse;
        public D3DCOLORVALUE Ambient;
        public D3DCOLORVALUE Specular;
        public D3DCOLORVALUE Emissive;
        public float Power;
        public D3DMATERIAL9(BinaryReader binaryReader)
        {
            Diffuse = new D3DCOLORVALUE(binaryReader);
            Ambient = new D3DCOLORVALUE(binaryReader);
            Specular = new D3DCOLORVALUE(binaryReader);
            Emissive = new D3DCOLORVALUE(binaryReader);
            Power = binaryReader.ReadSingle();
        }
    }

    class MATERIAL_BLOCK
    {
        const int MAX_VS_BONE = 28;
        int m_nStartVertex;         
        int m_nPrimitiveCount;      
        int m_nTextureID;           
        //	int		m_n2Side;				// 2 side render block
        //	int		m_nReflect;
        //	int		m_nOpacity;
        uint m_dwEffect;
        int m_nAmount;      // ������
        int m_nMaxUseBone;
        int[] m_UseBone;
        public MATERIAL_BLOCK(BinaryReader binaryReader)
        {
            m_nStartVertex = binaryReader.ReadInt32();
            m_nPrimitiveCount = binaryReader.ReadInt32();
            m_nTextureID = binaryReader.ReadInt32();
            m_dwEffect = binaryReader.ReadUInt32();
            m_nAmount = binaryReader.ReadInt32();
            m_nMaxUseBone = binaryReader.ReadInt32();
            m_UseBone = new int[MAX_VS_BONE];
            for(int i = 0;i < MAX_VS_BONE; i++)
                m_UseBone[i] = binaryReader.ReadInt32();
        }
        public static MATERIAL_BLOCK[] ReadBulk(BinaryReader binaryReader,int count)
        {
            var material_blocks = new MATERIAL_BLOCK[count];
            for (int i = 0; i < count; i++)
                material_blocks[i] = new MATERIAL_BLOCK(binaryReader);
            return material_blocks;
        }
    };
    class SKINVERTEX : NORMALVERTEX
    {
        public float w1, w2;       // vertex weight
        public uint matIdx;
        public SKINVERTEX(BinaryReader binaryReader)
        {
            position = new D3DXVECTOR3(binaryReader);
            w1 = binaryReader.ReadSingle();
            w2 = binaryReader.ReadSingle();
            matIdx = binaryReader.ReadUInt32();
            normal = new D3DXVECTOR3(binaryReader);
            tu = binaryReader.ReadSingle();
            tv = binaryReader.ReadSingle();
        }
        public static new SKINVERTEX[] ReadBulk(BinaryReader binaryReader, int count)
        {
            var vertices = new SKINVERTEX[count];
            for (int i = 0; i < count; i++)
                vertices[i] = new SKINVERTEX(binaryReader);
            return vertices;
        }
    };
    class NORMALVERTEX
    {
        public D3DXVECTOR3 position;   // The 3D position for the vertex
        public D3DXVECTOR3 normal;     // The surface normal for the vertex
        public float tu, tv;       // The texture coordinates
        public NORMALVERTEX()
        {

        }
        public NORMALVERTEX(BinaryReader binaryReader)
        {
            position = new D3DXVECTOR3(binaryReader);
            normal = new D3DXVECTOR3(binaryReader);
            tu = binaryReader.ReadSingle();
            tv = binaryReader.ReadSingle();
        }
        public static NORMALVERTEX[] ReadBulk(BinaryReader binaryReader, int count)
        {
            var vertices = new NORMALVERTEX[count];
            for (int i = 0; i < count; i++)
                vertices[i] = new NORMALVERTEX(binaryReader);
            return vertices;
        }
    };
}
