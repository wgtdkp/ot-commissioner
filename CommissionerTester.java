class MyHandler extends CommissionerHandler {
    @Override
    public void OnLogging(LogLevel aLevel, String aMsg) {
        System.out.printf("[ot-commissioner] [%s]: %s\n", aLevel.toString(), aMsg);
    }
};

public class CommissionerTester {

    private MyHandler myCommissionerHandler;
    private Commissioner commissioner;

    private void runTest() {
        Config config = new Config();

        config.setId("TestComm");
        config.setEnableCcm(false);
        config.setDomainName("TestDomain");
        config.setEnableDtlsDebugLogging(true);
        config.setLogLevel(LogLevel.kDebug);
        config.setPSKc(new ByteArray(new short[]{0x3a, 0xa5, 0x5f, 0x91, 0xca, 0x47, 0xd1, 0xe4, 0xe7, 0x1a, 0x08, 0xcb, 0x35, 0xe9, 0x15, 0x91}));

        myCommissionerHandler = new MyHandler();

        commissioner = Commissioner.Create(myCommissionerHandler, config);
        commissioner.Start();

        String[] existingCommissionerId = new String[1];
        Error error = commissioner.Petition(existingCommissionerId, "2401:fa00:41:23:cdb3:b615:99d9:ee8b", 49191);

        boolean active = commissioner.IsActive();
        System.out.println("active: " + active);

        commissioner.Stop();
    }

    public static void main(String[] args) {
        CommissionerTester tester = new CommissionerTester();
        tester.runTest();
    }
}
