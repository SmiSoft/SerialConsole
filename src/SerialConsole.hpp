#ifndef COMCONSOLE_HPP_GUARD
#define COMCONSOLE_HPP_GUARD

boolean isPrintable(char ch);

class SerialConsole
{
  private:
    const char*promt;
    byte bufferSize;
    byte bufferPos;
    Stream&serial;
    char*buffer1;
    char*buffer2;
    boolean requiredProcess;
  public:
    // _serial - linked to this console serial port
    // _bufferSize - input buffer size
    // _promt - prompt for the interactive mode
    SerialConsole(Stream&_serial, byte _bufferSize, const char*_promt);
    ~SerialConsole();
    // placeholder for serialEvent
    void nextChar();
    // command from user or NULL if no user input
    const char*avail();
    // call after user's input processing (display promt)
    void done();
};

#endif
