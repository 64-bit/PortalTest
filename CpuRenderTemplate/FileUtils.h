#pragma once

template <typename T>
T* ReadTypedArrayFromFile(FILE* file, size_t count) {
    T * result = new T[count];
    fread(result, sizeof(T), count, file);
    return result;
}

template <typename T>
void ReadTypedArrayFromFile(FILE* file, size_t count, T* dst) {
    fread(dst, sizeof(T), count, file);
}

template <typename T>
T ReadTypedObjectFromFile(FILE* file) {
    T result;
    fread(&result, sizeof(T), 1, file);
    return result;
}