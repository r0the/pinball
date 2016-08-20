#ifndef SD_H
#define SD_H

class SDCard {
public:
    File open(const char* filename, uint8_t mode) {
        QFile* file = new QFile(filename);
        if (!file->open(QIODevice::ReadOnly)) {
            qDebug() << "Error while opening file " << filename;
        }

        return File(file);
    }
};

extern SDCard SD;

#endif // SD_H
