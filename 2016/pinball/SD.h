#ifndef SD_H
#define SD_H

class File {
public:
    File() {
    }

    File(QFile* handle) :
        _handle(handle) {
    }

    ~File() {
        delete _handle;
    }

    qint64 available() const {
        return _handle->bytesAvailable();
    }

    void close() {
        _handle->close();
    }

    char read() {
        char result;
        _handle->getChar(&result);
        return result;
    }

    int read(uint8_t*, int) {
        // TODO
    }

private:
    QFile* _handle;
};

class SDCard {
public:
    bool exists(const char* filename) {
        return false;
    }

    File open(const char* filename, uint8_t mode) {
        QFile* file;
        if (QString(filename) == QString("p.txt")) {
            file = new QFile("../pinball/p.txt");
        }
        else {
            file = new QFile(filename);
        }

        if (!file->open(QIODevice::ReadOnly)) {
            qDebug() << "Error while opening file " << filename;
        }

        return File(file);
    }
};

extern SDCard SD;

#endif // SD_H
