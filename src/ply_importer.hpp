#ifndef SIMD_EXPERIMENT_PLY_IMPORTER_HPP
#define SIMD_EXPERIMENT_PLY_IMPORTER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <iostream>

#include "raster.hpp"

bool ply_import(const std::string &path, std::vector<Patch> &patches);

#endif
