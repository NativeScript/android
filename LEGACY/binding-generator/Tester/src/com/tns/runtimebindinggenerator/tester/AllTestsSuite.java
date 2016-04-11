package com.tns.runtimebindinggenerator.tester;

import android.test.suitebuilder.TestSuiteBuilder;
import junit.framework.Test;
import junit.framework.TestSuite;

public class AllTestsSuite extends TestSuite
{
	public static Test suite()
	{
		return new TestSuiteBuilder(AllTestsSuite.class).includeAllPackagesUnderHere().build();
	}
}
