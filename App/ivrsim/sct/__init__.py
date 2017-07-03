import unittest;

from sct     import test_ivrsim     as ivrsim_sct_tests;


if __name__ == "__main__":
    print("SCT-tests are started...");
    
    suite = unittest.TestSuite();

    suite.addTests(unittest.TestLoader().loadTestsFromModule(ivrsim_sct_tests));
    result = unittest.TextTestRunner(verbosity = 2).run(suite);
    
    # Get execution result
    execution_testing_result = 0;
    
    if (result.wasSuccessful() is True):
        execution_testing_result = 0;
    else:
        execution_testing_result = 1;
    
    exit(execution_testing_result);