#include "ply_importer.hpp"

struct Vertex {
    f32 x{}, y{}, z{};
    f32 nx{}, ny{}, nz{};
    u8 r{}, g{}, b{};
};

struct PLYHeader {
    u32 vertex_count{};
    u32 face_count{};
};

static std::vector<char> read_file_buf(const std::string &path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        std::cout << "Failed to read file from: \"" << path << "\"\n";
        return std::vector<char>{};
    }

    usize file_size = file.tellg();
    std::vector<char> buffer(static_cast<usize>(file_size));

    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(file_size));
    file.close();

    return buffer;
}

static size_t read_line(std::string &out_line, const std::vector<char> &vec, size_t start) {
    std::string line{};

    for (; start < vec.size(); ++start) {
        if (vec[start] == '\n') {
            break;
        }

        line.push_back(vec[start]);
    }

    out_line = line;

    return start + 1;
}

static bool parse_ply_header(const std::vector<char> &buffer, PLYHeader &header, size_t &end_idx) {
    std::string line{};
    size_t idx{};

    idx = read_line(line, buffer, idx);
    if (line != "ply") {
        std::cout << "Wrong ply header start!\n";
        std::cout << "\"" << line << "\"\n";
        return false;
    }

    idx = read_line(line, buffer, idx);
    if (line != "format binary_little_endian 1.0") {
        std::cout << "Unsupported ply header format!\n";
        std::cout << "\"" << line << "\"\n";
        return false;
    }

    do {
        idx = read_line(line, buffer, idx);
    } while (!line.starts_with("element"));

    if (line.starts_with("element vertex")) {
        header.vertex_count = static_cast<usize>(std::stoi(line.substr(std::string{"element vertex "}.size())));
        std::cout << "Vert count: " << header.vertex_count << '\n';

        idx = read_line(line, buffer, idx);
        if (line != "property float x") {
            std::cout << "Expected element vertex to have property: \"float x\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property float y") {
            std::cout << "Expected element vertex to have property: \"float y\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property float z") {
            std::cout << "Expected element vertex to have property: \"float z\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property float nx") {
            std::cout << "Expected element vertex to have property: \"float nx\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property float ny") {
            std::cout << "Expected element vertex to have property: \"float ny\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property float nz") {
            std::cout << "Expected element vertex to have property: \"float nz\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property uchar red") {
            std::cout << "Expected element vertex to have property: \"uchar red\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property uchar green") {
            std::cout << "Expected element vertex to have property: \"uchar green\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property uchar blue") {
            std::cout << "Expected element vertex to have property: \"uchar blue\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

        idx = read_line(line, buffer, idx);
        if (line != "property uchar alpha") {
            std::cout << "Expected element vertex to have property: \"uchar alpha\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }

    } else {
        std::cout << "Unexpected ply header element! Expected vertex.\n";
        std::cout << "\"" << line << "\"\n";
        return false;
    }

    do {
        idx = read_line(line, buffer, idx);
    } while (!line.starts_with("element"));

    if (line.starts_with("element face")) {
        header.face_count = static_cast<usize>(std::stoi(line.substr(std::string{"element face "}.size())));
        std::cout << "Face count: " << header.face_count << '\n';

        idx = read_line(line, buffer, idx);
        if (line != "property list uchar uint vertex_indices") {
            std::cout << "Expected element vertex to have property: \"uchar alpha\"\n";
            std::cout << "\"" << line << "\"\n";
            return false;
        }
    } else {
        std::cout << "Unexpected ply header element! Expected face.\n";
        std::cout << "\"" << line << "\"\n";
        return false;
    }

    idx = read_line(line, buffer, idx);
    if (line != "end_header") {
        std::cout << "Wrong ply header end! ";
        std::cout << "\"" << line << "\"\n";
        return false;
    }

    end_idx = idx;

    return true;
}
static bool parse_vertices_bin(const std::vector<char> &buffer, std::vector<Vertex> &vertices, size_t &end_idx, const PLYHeader &header) {
    const usize PLY_VERT_SIZE = sizeof(f32) * 3 + sizeof(f32) * 3 + sizeof(u8) * 4;

    for (usize i{}; i < header.vertex_count; ++i) {
        Vertex vert{};
        std::memcpy(&vert, &buffer[end_idx + (i * PLY_VERT_SIZE)], sizeof(Vertex));

        vert.x *= -1.0f;

        vertices.push_back(vert);
    }

    end_idx += header.vertex_count * PLY_VERT_SIZE;

    return true;
}
static bool parse_patches_bin(const std::vector<char> &buffer, std::vector<Patch> &patches, size_t &end_idx, const std::vector<Vertex> &vertices, const PLYHeader &header) {
    const usize PLY_FACE_SIZE = sizeof(u8) + sizeof(u32) * 3;

    for (usize i{}; i < header.face_count; ++i) {
        u8 face_size{};
        u32 face_ind[3];

        std::memcpy(&face_size, &buffer[end_idx + (i * PLY_FACE_SIZE)], sizeof(face_size));

        if (face_size != 3) {
            std::cout << "All faces must be triangulated! face[" << i << "] consists of " << (u32) face_size << " vertices!\n";
            return false;
        }

        std::memcpy(face_ind, &buffer[end_idx + (i * PLY_FACE_SIZE) + sizeof(face_size)], sizeof(face_ind[0]) * 3);

        const Vertex &v0 = vertices[static_cast<usize>(face_ind[2])];
        const Vertex &v1 = vertices[static_cast<usize>(face_ind[1])];
        const Vertex &v2 = vertices[static_cast<usize>(face_ind[0])];

        patches.push_back(Patch{
            .pos {
                vec4(v0.x, v0.y, v0.z, 1.0f),
                vec4(v1.x, v1.y, v1.z, 1.0f),
                vec4(v2.x, v2.y, v2.z, 1.0f),
            },
            .normal = (vec3(v0.nx, v0.ny, v0.nz) + vec3(v1.nx, v1.ny, v1.nz) + vec3(v2.nx, v2.ny, v2.nz)) / 3.0f,
            .color = (
                vec3((f32) v0.r / 255.0f, (f32) v0.g / 255.0f, (f32) v0.b / 255.0f) +
                vec3((f32) v1.r / 255.0f, (f32) v1.g / 255.0f, (f32) v1.b / 255.0f) +
                vec3((f32) v2.r / 255.0f, (f32) v2.g / 255.0f, (f32) v2.b / 255.0f)
            ) / 3.0f
        });
    }

    return true;
}

bool ply_import(const std::string &path, std::vector<Patch> &patches) {
    std::vector<char> buffer = read_file_buf(path);

    size_t buf_idx{};

    PLYHeader header{};
    if (!parse_ply_header(buffer, header, buf_idx)) {
        std::cout << "Failed to parse the ply header!\n";
        return false;
    }

    std::vector<Vertex> vertices{};
    if (!parse_vertices_bin(buffer, vertices, buf_idx, header)) {
        std::cout << "Failed to parse the vertex data!\n";
        return false;
    }

    if (!parse_patches_bin(buffer, patches, buf_idx, vertices, header)) {
        std::cout << "Failed to parse the patch data!\n";
        return false;
    }

    return true;
}