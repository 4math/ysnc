#include <iostream>
#include <filesystem>
#include "tokenizer.h"
#include "moss.h"
#include "jaccard_index.h"
#include "output_html.h"
#include "console.h"

namespace fs = std::filesystem;

int main() {
    ConsoleUI console;
    console.run();

    return 0;
}
