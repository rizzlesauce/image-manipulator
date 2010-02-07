#ifndef Debugger_h
#define Debugger_h

#include <string>
#include <QObject>

using namespace std;

/**
 * Debugger
 *
 * This class handles debugging messages and options for the program. It is a singleton
 */
class Debugger : public QObject {
    Q_OBJECT

    public:
    /**
     * Get the only instance of this class
     */
    static Debugger& getInstance();
    bool isEnabled();
    void setEnabled(bool = true);
    void print(std::string);

    signals:
    void messagePrinted(QString message);

    private:
    Debugger();

    protected:
    bool enabled_;
};

#endif
