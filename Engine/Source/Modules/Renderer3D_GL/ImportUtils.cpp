#include "Modules/VhModules_PCH.hpp"
#include "ImportUtils.hpp"

const size_t MAX_LEN = 512;

using vh::V3;

namespace
{

// Check that objFile is a valid Wavefront .obj file
bool checkFilename(const char *objFilename)
{
    size_t len = strlen(objFilename);
    if (len > MAX_LEN)
    {
        LOG(ERROR) << "Filename too long: " << objFilename;
        return false;
    }
    return strncmp(objFilename + (len - 4), ".obj", 4) == 0;
}

// Get .mtl objFile for specified .obj file
void getMtlFilename(char *mtlFilename, const char *objFilename)
{
    size_t len = strlen(objFilename);
    strncpy_s(mtlFilename, MAX_LEN, objFilename, MAX_LEN);
    strncpy_s(mtlFilename + (len - 4), MAX_LEN, ".mtl", 4);
}

void readMaterials(FILE *mtlFile,
    std::unordered_map<std::string, V3> &aColorMap,
    std::unordered_map<std::string, V3> &dColorMap,
    std::unordered_map<std::string, V3> &sColorMap,
    std::unordered_map<std::string, std::string> &textures)
{
    char line[MAX_LEN], material[MAX_LEN];
    V3 vec3;

    char texFile[MAX_LEN];

    while (fgets(line, MAX_LEN, mtlFile) != NULL)
    {
        sscanf_s(line, "newmtl %s", material, (unsigned) MAX_LEN);
        if (sscanf_s(line, "Ka %f %f %f", &vec3.x, &vec3.y, &vec3.z))
        {
            aColorMap.insert(std::pair<std::string, V3>(std::string(material), vec3));
            LOG(INFO) << material << ": " << glm::to_string(vec3);
        }
        if (sscanf_s(line, "Kd %f %f %f", &vec3.x, &vec3.y, &vec3.z))
        {
            dColorMap.insert(std::pair<std::string, V3>(std::string(material), vec3));
            LOG(INFO) << material << ": " << glm::to_string(vec3);
        }
        if (sscanf_s(line, "Ks %f %f %f", &vec3.x, &vec3.y, &vec3.z))
        {
            sColorMap.insert(std::pair<std::string, V3>(std::string(material), vec3));
            LOG(INFO) << material << ": " << glm::to_string(vec3);
        }
        if (sscanf_s(line, "map_Kd %s", texFile, (unsigned) MAX_LEN))
        {
            textures[std::string(material)] = std::string(texFile);
            LOG(INFO) << material << ": texture " << texFile;
        }
    }
}

}

/**
 * Constructor
 **/
bool vh::Utils::ImportWavefront(vh::Mesh* mesh, const char* objFilename)
{
    CHECK(mesh);

    // Open objFile specified by 'objFilename' (should and with .obj)
    if (!checkFilename(objFilename))
    {
        LOG(ERROR) << "Invalid objFilename (should end with .obj): " << objFilename;
        return false;
    }

    FILE* objFile;
    errno_t err = fopen_s(&objFile, objFilename, "r");

    if (err)
    {
        LOG(ERROR) << "Can't open objFile: " << objFilename;
        return false;
    }

    // File opened successfully, go on
    LOG(INFO) << "Reading objFile " << objFilename;

    // Try to open material library for this object
    char mtlFilename[MAX_LEN];
    getMtlFilename(mtlFilename, objFilename);

    FILE* mtlFile;
    err = fopen_s(&mtlFile, mtlFilename, "r");

    if (err)
    {
        LOG(INFO) << "No material library found at " << mtlFilename;
    }
    else
    {
        LOG(INFO) << "Found material library file: " << mtlFilename;
    }

    std::vector<V3> vertices, normals, aColor, dColor, sColor;
    std::vector<V3> uvs; // U, V, texIdx

    char line[MAX_LEN] = { 0 }, temp[MAX_LEN] = "";
    std::string mtl = "";
    char s1[32], s2[32], s3[32];
    V3 vec3;
    V2 vec2;
    std::unordered_map<std::string, size_t> map;
    std::vector<std::string> values;
    std::vector<size_t> indices;

    std::unordered_map<std::string, size_t> materials;
    std::unordered_map<std::string, V3> mtlAColor, mtlDColor, mtlSColor;

    std::unordered_map<std::string, std::string> mtlTex;

    if (mtlFile != NULL)
    {
        readMaterials(mtlFile, mtlAColor, mtlDColor, mtlSColor, mtlTex);
    }

    V3 aCol, dCol, sCol;

    std::vector<std::string> textures;
    size_t currTex = 0;

    if (mtlTex.size())
    {
        textures.push_back(mtlTex.begin()->second);
    }

    while (!feof(objFile))
    {

        // read next line
        if (fgets(line, MAX_LEN, objFile) == NULL) break;

        if (sscanf_s(line, "usemtl %s", temp, (unsigned) MAX_LEN))
        {
            mtl = temp;
            LOG(INFO) << "Using material: " << mtl;
            map.clear(); // clear the map, so vertices with different material won't get the same index
            aCol = mtlAColor[mtl];
            dCol = mtlDColor[mtl];
            sCol = mtlSColor[mtl];
            currTex = 0;
        }

        if (sscanf_s(line, "v %f %f %f", &vec3.x, &vec3.y, &vec3.z))
        {
            vertices.push_back(vec3);
        }
        else if (sscanf_s(line, "vt %f %f", &vec3.x, &vec3.y))
        {
            vec3.z = (float) currTex;
            uvs.push_back(vec3);

        }
        else if (sscanf_s(line, "vn %f %f %f", &vec3.x, &vec3.y, &vec3.z))
        {
            normals.push_back(vec3);
        }
        else if (sscanf_s(line, "f %s %s %s", s1, (unsigned)MAX_LEN, s2, (unsigned)MAX_LEN, s3, (unsigned)MAX_LEN))
        {
            // finding vertices with identical values for position/normals/uv (and material)
            if (map.count(std::string(s1)))
            {
                indices.push_back(map[s1]);
            }
            else
            {
                values.push_back(std::string(s1));
                if (!mtl.empty())
                {
                    aColor.push_back(aCol);
                    dColor.push_back(dCol);
                    sColor.push_back(sCol);
                }
                map.insert(std::pair<std::string, size_t>(s1, values.size() - 1));
                indices.push_back(values.size() - 1);
            }
            if (map.count(std::string(s2)))
            {
                indices.push_back(map[s2]);
            }
            else
            {
                values.push_back(std::string(s2));
                if (!mtl.empty())
                {
                    aColor.push_back(aCol);
                    dColor.push_back(dCol);
                    sColor.push_back(sCol);
                }
                map.insert(std::pair<std::string, size_t>(s2, values.size() - 1));
                indices.push_back(values.size() - 1);
            }
            if (map.count(std::string(s3)))
            {
                indices.push_back(map[s3]);
            }
            else
            {
                values.push_back(std::string(s3));
                if (!mtl.empty())
                {
                    aColor.push_back(aCol);
                    dColor.push_back(dCol);
                    sColor.push_back(sCol);
                }
                map.insert(std::pair<std::string, size_t>(s3, values.size() - 1));
                indices.push_back(values.size() - 1);
            }
        }
    }

    //  LOG(INFO) << "aColor size: " << aColor.size();
    //  LOG(INFO) << "dColor size: " << dColor.size();
    //  LOG(INFO) << "sColor size: " << sColor.size();

    if (mtlFile != NULL) fclose(mtlFile);
    fclose(objFile);

    // Allocate attrib data
    GLuint attribCount = 1;                     // normals are always present
    if (aColor.size() > 0) attribCount += 3;    // add color triad if specified
    if (uvs.size() > 0) attribCount += 1;       // add UV coordinates if specified

    GLuint vertexNum = (GLuint) values.size();
    GLuint attribSize = (GLuint) values.size() * (3 + 3 * attribCount);      // for each vertex: vec3 coordinates + vec3 * attribCount
    GLfloat* attribData = new GLfloat[attribSize];

    // Allocate index data
    GLuint indexSize = (GLuint) indices.size();
    GLuint* indexData = new GLuint[indexSize];

    Uint32 v = 0, vt = 0, vn = 0;
    std::string str;
    GLfloat *base;

    // Fill vertex data and normal data
    for (size_t i = 0; i < values.size(); i++)
    {
        str = values[i];
        if (str.find("//") != std::string::npos)
        {
            sscanf_s(str.c_str(), "%d//%d", &v, &vn);
        }
        else
        {
            sscanf_s(str.c_str(), "%d/%d/%d", &v, &vt, &vn);
        }

        // Vertices
        base = attribData + 3 * i;
        if (v > 0 && v <= vertices.size())
        {
            base[0] = vertices[v - 1].x;
            base[1] = vertices[v - 1].y;
            base[2] = vertices[v - 1].z;
        }

        // Normals
        base += (3 * vertexNum);
        if (vn > 0 && vn <= normals.size())
        {
            base[0] = normals[vn - 1].x;
            base[1] = normals[vn - 1].y;
            base[2] = normals[vn - 1].z;
        }

        // Ambient colors
        base += (3 * vertexNum);
        if (i < aColor.size())
        {
            base[0] = aColor[i].x;
            base[1] = aColor[i].y;
            base[2] = aColor[i].z;
        }

        // Diffuse colors
        base += (3 * vertexNum);
        if (i < dColor.size())
        {
            base[0] = dColor[i].x;
            base[1] = dColor[i].y;
            base[2] = dColor[i].z;
        }

        // Specular colors
        base += (3 * vertexNum);
        if (i < sColor.size())
        {
            base[0] = sColor[i].x;
            base[1] = sColor[i].y;
            base[2] = sColor[i].z;
        }

        // uv coords
        base += (3 * vertexNum);
        if (vt > 0 && vt <= uvs.size())
        {
            base[0] = uvs[vt - 1].x;
            base[1] = 1.0f - uvs[vt - 1].y;
            base[2] = uvs[vt - 1].z;
        }
    }

    for (size_t i = 0; i < indices.size(); i++)
    {
        indexData[i] = (GLuint) indices[i];
    }

    // Set attrib and index data to mesh
    mesh->SetAttribData(vertexNum, attribCount, attribData);
    mesh->SetIndexData(indexSize, indexData);

    for (size_t idx = 0; idx < textures.size(); ++idx)
    {
        SDL_Surface* surf = IMG_Load(textures[idx].c_str());
        if (surf == nullptr)
        {
            LOG(ERROR) << "Unable to load texture " << textures[idx];
        }
        else
        {
            uint32_t* dta = Utils::ConvertToRGBA(surf);
            mesh->SetTexture(dta, surf->w, surf->h);
            SDL_FreeSurface(surf);
        }
    }

    LOG(INFO) << "v: " << vertices.size();
    LOG(INFO) << "vn: " << normals.size();
    LOG(INFO) << "vt: " << uvs.size();
    LOG(INFO) << "values: " << values.size();

    return true;
}
