#include <QtTest/QtTest>
#include "compiler/CompilerRegistry.h"

class CompilerRegistryTest : public QObject
{
    Q_OBJECT

private slots:
    void testAutoScan()
    {
        CompilerRegistry::instance().autoScanCompilers();
        auto compilers = CompilerRegistry::instance().getAvailableCompilers();
        
        // Should find at least one compiler on most systems
        QVERIFY(!compilers.isEmpty());
    }
    
    void testCompilerProperties()
    {
        CompilerRegistry::instance().autoScanCompilers();
        auto compilers = CompilerRegistry::instance().getAvailableCompilers();
        
        if (!compilers.isEmpty()) {
            auto compiler = compilers.first();
            
            // Check basic properties
            QVERIFY(!compiler->id().isEmpty());
            QVERIFY(!compiler->name().isEmpty());
            QVERIFY(!compiler->executablePath().isEmpty());
            QVERIFY(compiler->isAvailable());
            QVERIFY(!compiler->supportedStandards().isEmpty());
        }
    }
};

QTEST_MAIN(CompilerRegistryTest)
#include "CompilerRegistryTest.moc"
