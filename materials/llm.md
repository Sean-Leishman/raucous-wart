# Question and Response
1. Create code for a class for an image in ppm format

Certainly! The PPM (Portable Pixmap) format is a simple image format which can be represented in ASCII (P3) or binary (P6). Here, I'll provide a basic C++ class to handle the ASCII (P3) version of the PPM format. The P3 format begins with the "P3" magic number, followed by the image width, height, and maximum color value (usually 255 for 8-bit per channel), and then the pixel data in the order of red, green, and blue.

```cpp
#include <vector>
#include <fstream>
#include <iostream>

class PPMImage {
private:
    int width;
    int height;
    int maxColorValue;
    std::vector<std::vector<std::vector<int>>> data; // data[y][x][channel]

public:
    PPMImage(int width, int height, int maxColorValue = 255)
        : width(width), height(height), maxColorValue(maxColorValue) {
        data.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(3)));
    }

    void setPixel(int x, int y, int r, int g, int b) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        data[y][x][0] = r;
        data[y][x][1] = g;
        data[y][x][2] = b;
    }

    bool saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return false;
        }

        // Write header
        file << "P3\n";
        file << width << " " << height << "\n";
        file << maxColorValue << "\n";

        // Write pixel data
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file << data[y][x][0] << " " << data[y][x][1] << " " << data[y][x][2] << "\n";
            }
        }

        file.close();
        return true;
    }
};

// Usage example
int main() {
    PPMImage img(100, 100);

    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            img.setPixel(x, y, 255, 0, 0); // Setting all pixels to red for demonstration
        }
    }

    img.saveToFile("output.ppm");
    return 0;
}
```

This class allows you to create a PPM image of a specified size, set pixel values, and save the image to a file. You can expand this to include methods for reading from a PPM file, error-checking, etc. The usage example creates a 100x100 red image and saves it as "output.ppm".

2. Parse JSON with a cpp class and store it in some relevant format

Parsing JSON in C++ typically involves using a library, as C++ doesn't have a built-in JSON parser. One of the most popular libraries for this purpose is `nlohmann/json`. Here's how you can create a C++ class that uses this library to parse JSON and store it in a relevant format:

1. First, you need to install and set up `nlohmann/json`. If you're using package managers like `vcpkg` or `conan`, they can be handy. Otherwise, you can directly include the single-header version available at the [nlohmann/json](https://github.com/nlohmann/json) GitHub repository.

2. Once set up, here's a simple example class to parse a JSON:

```cpp
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

class JSONParser {
private:
    nlohmann::json jsonData;

public:
    JSONParser(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> jsonData;
        }
    }

    // Retrieve a value by key
    template <typename T>
    T get(const std::string& key) const {
        if (jsonData.contains(key)) {
            return jsonData.at(key).get<T>();
        }
        throw std::runtime_error("Key not found");
    }

    // Example: Check if a key exists
    bool contains(const std::string& key) const {
        return jsonData.contains(key);
    }

    // For demonstration purposes: Print the entire JSON
    void print() const {
        std::cout << jsonData.dump(4) << std::endl;
    }
};

int main() {
    JSONParser parser("sample.json");

    if (parser.contains("name")) {
        std::string name = parser.get<std::string>("name");
        std::cout << "Name: " << name << std::endl;
    }

    parser.print();

    return 0;
}
```

This example assumes a `sample.json` file like:
```json
{
    "name": "John",
    "age": 30
}
```

With this `JSONParser` class, you can read from a JSON file, retrieve values by their keys, and print the entire JSON content.

Note: JSON parsing and manipulation with `nlohmann/json` are much richer than demonstrated. This example provides a starting point, but you can extend and customize it to fit more advanced requirements.