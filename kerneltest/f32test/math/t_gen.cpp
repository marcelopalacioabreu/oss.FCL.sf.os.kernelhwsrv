// Copyright (c) 1996-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// f32test\math\t_gen.cpp
// Generates files to be used in testing whether arithmetic in TReal32 and TReal64 gives
// exactly the same results using the MSDEV complier as on the rack.
// 
//

#include <e32test.h>
#include <f32file.h>
#include <e32svr.h>
#include "../server/t_server.h"
#include "t_math.h"

GLDEF_D RTest test(_L("T_GEN"));

LOCAL_D const TFileName pathName = _L("C:\\F32-TST\\E32-MATH\\");
LOCAL_D	TBuf<0x100> buf;
LOCAL_D	TBuf8<0x100> buf8;


#define __GENERATE_SPECIAL_VALUES
#ifdef __GENERATE_SPECIAL_VALUES

LOCAL_D	RFile fspw;	// file for special values for WINS
LOCAL_D	RFile fspa;	// file for special values for ARM

LOCAL_D const TFileName fileNameWSp = _L("T_VALS.HWG");
LOCAL_D const TFileName fileNameASp = _L("T_VALS.HAG");

LOCAL_C void appendConstTI64(TPtrC aName,TReal64* aVal)
	{
	// for WINS 
	buf=_L("");
	buf.AppendFormat(_L("const TInt64 %S = TInt64(0x%x,0x%x);\n"),&aName,*(((TUint32*)aVal) + 1),*(TUint32*)aVal);
	buf8.Copy(buf);	// Unicode
	fspw.Write(buf8);
	// for ARM
	buf=_L("");
	buf.AppendFormat(_L("const TInt64 %S = TInt64(0x%x,0x%x);\n"),&aName,*(TUint32*)aVal,*(((TUint32*)aVal) + 1));
	buf8.Copy(buf);
	fspa.Write(buf8);
	}

LOCAL_C void appendConstTI32x(TPtrC aName,TReal32* aVal)
	{
	// for WINS and ARM
	buf=_L("");
	buf.AppendFormat(_L("const TInt32x %S = TInt32x(0x%x);\n"),&aName,*(TUint32*)aVal);
	buf8.Copy(buf);	// Unicode
	fspw.Write(buf8);	
	fspa.Write(buf8);	
	}

LOCAL_C void appendConstTR64(TPtrC aName,TPtrC aIntName)
	{
	// for WINS and ARM
	buf=_L("");
	buf.AppendFormat(_L("const TReal64 %S = *(TReal64*)&%S;\n"),&aName,&aIntName);
	buf8.Copy(buf);	// Unicode
	fspw.Write(buf8);
	fspa.Write(buf8);
	}

LOCAL_C void appendConstTR32(TPtrC aName,TPtrC aIntName)
	{
	// for WINS and ARM
	buf=_L("");
	buf.AppendFormat(_L("const TReal32 %S = *(TReal32*)&%S;\n"),&aName,&aIntName);
	buf8.Copy(buf);
	fspw.Write(buf8);	
	fspa.Write(buf8);	
	}

LOCAL_C void appendConstTR(TPtrC aName,TPtrC a64Name)
	{ 
	// for WINS and ARM
	buf=_L("");
	buf.AppendFormat(_L("const TReal %S = TReal(%S);\n"),&aName,&a64Name);
	buf8.Copy(buf);	// Unicode
	fspw.Write(buf8);	
	fspa.Write(buf8);	
	}

LOCAL_C void appendStart()
	{
	// for WINS
	fspw.Write(_L8("/* Generated by \\E32\\TMATH\\T_GEN.MAK for WINS (little-endian doubles) */\n\n"));
	fspw.Write(_L8("#include <e32std.h>\n\n"));
	fspw.Write(_L8("class TInt32x\n{\npublic:\n"));
	fspw.Write(_L8("TInt32x(TInt32 aInt) {iInt=aInt;}\n"));
	fspw.Write(_L8("public:\nTInt32 iInt;\n};\n\n"));

	// for ARM
	fspa.Write(_L8("/* Generated by \\E32\\TMATH\\T_GEN.MAK for ARM (big-endian doubles)*/\n\n"));
	fspa.Write(_L8("#include <e32std.h>\n\n"));
	fspa.Write(_L8("class TInt32x\n{\npublic:\n"));
	fspa.Write(_L8("TInt32x(TInt32 aInt) {iInt=aInt;}\n"));
	fspa.Write(_L8("public:\nTInt32 iInt;\n};\n\n"));
	}

LOCAL_C void generateSpecialValues()
//
// The following are to avoid the errors in the compiler in converting from string
// to floating point
//
// Initialise max/min values and square roots and write to header files
//
	{

	TReal64 minTReal32in64;
	SReal64 *p64=(SReal64*)&minTReal32in64;
	p64->sign=0;
	p64->exp=0x381;
	p64->msm=0;
	p64->lsm=0;

	TReal64 maxTReal32in64;
	p64=(SReal64*)&maxTReal32in64;
	p64->sign=0;
	p64->exp=0x47E;
	p64->msm=0xfffff;
	p64->lsm=0xe0000000;

	TReal64 sqrtMaxTReal64;		
	const TReal64 arg1=KMaxTReal64-1.0E+299;		// Take off this to avoid overflow
	TInt ret=Math::Sqrt(sqrtMaxTReal64,arg1);
	TReal64 sqrtMinTReal64;
	const TReal64 arg2=KMinTReal64;
	ret=Math::Sqrt(sqrtMinTReal64,arg2);

	TReal64 sqrtMaxTReal32in64,sqrtMinTReal32in64; 

	TReal64 negZeroTReal64;
	p64=(SReal64*)&negZeroTReal64;
	p64->sign=1;
	p64->exp=0;
	p64->msm=0;
	p64->lsm=0;

	TReal64 posInfTReal64;
	p64=(SReal64*)&posInfTReal64;
	p64->sign=0;
	p64->exp=KTReal64SpecialExponent;
	p64->msm=0;
	p64->lsm=0;

	TReal64 negInfTReal64;
	p64=(SReal64*)&negInfTReal64;
	p64->sign=1;
	p64->exp=KTReal64SpecialExponent;
	p64->msm=0;
	p64->lsm=0;

	TReal64 NaNTReal64;
	p64=(SReal64*)&NaNTReal64;
	p64->sign=0;
	p64->exp=KTReal64SpecialExponent;
	p64->msm=0xfffffu;
	p64->lsm=0xffffffffu;

	TReal32 sqrtMaxTReal32;
	const TReal arg3=maxTReal32in64-1.0E+32f;		// Take off this to avoid overflow
	ret=Math::Sqrt(sqrtMaxTReal32in64,arg3);
	sqrtMaxTReal32=TReal32(sqrtMaxTReal32in64);
	
	TReal32 sqrtMinTReal32;
	const TReal arg4=minTReal32in64;
	ret=Math::Sqrt(sqrtMinTReal32in64,arg4);
	sqrtMinTReal32=TReal32(sqrtMinTReal32in64);

	TReal32 negZeroTReal32;
	SReal32 *p32=(SReal32*)&negZeroTReal32;
	p32->sign=1;
	p32->exp=0;
	p32->man=0;	

	appendStart();

	appendConstTI64(_L("minTReal32in64"),&minTReal32in64);
	appendConstTI64(_L("maxTReal32in64"),&maxTReal32in64);
	appendConstTI64(_L("sqrtMaxTReal64"),&sqrtMaxTReal64);
	appendConstTI64(_L("sqrtMinTReal64"),&sqrtMinTReal64);
	appendConstTI64(_L("negZeroTReal64"),&negZeroTReal64);
	appendConstTI64(_L("posInfTReal64"),&posInfTReal64);
	appendConstTI64(_L("negInfTReal64"),&negInfTReal64);
	appendConstTI64(_L("NaNTReal64"),&NaNTReal64);

	appendConstTI32x(_L("sqrtMaxTReal32"),&sqrtMaxTReal32);
	appendConstTI32x(_L("sqrtMinTReal32"),&sqrtMinTReal32);
	appendConstTI32x(_L("negZeroTReal32"),&negZeroTReal32);

	appendConstTR64(_L("KMinTReal32in64"),_L("minTReal32in64"));
	appendConstTR64(_L("KMaxTReal32in64"),_L("maxTReal32in64"));
	appendConstTR64(_L("KSqrtMaxTReal64"),_L("sqrtMaxTReal64"));
	appendConstTR64(_L("KSqrtMinTReal64"),_L("sqrtMinTReal64"));
	appendConstTR64(_L("KNegZeroTReal64"),_L("negZeroTReal64"));
	appendConstTR64(_L("KPosInfTReal64"),_L("posInfTReal64"));
	appendConstTR64(_L("KNegInfTReal64"),_L("negInfTReal64"));
	appendConstTR64(_L("KNaNTReal64"),_L("NaNTReal64"));
	
	appendConstTR32(_L("KSqrtMaxTReal32"),_L("sqrtMaxTReal32"));
	appendConstTR32(_L("KSqrtMinTReal32"),_L("sqrtMinTReal32"));
	appendConstTR32(_L("KNegZeroTReal32"),_L("negZeroTReal32"));

	appendConstTR(_L("KMinTReal32inTReal"),_L("KMinTReal32in64"));
	appendConstTR(_L("KMaxTReal32inTReal"),_L("KMaxTReal32in64"));
	appendConstTR(_L("KNegZeroTReal"),_L("KNegZeroTReal64"));
	}

#endif

//#define __GENERATE_TR64 
#ifdef __GENERATE_TR64

// Data for tests from T_R64DTA.cpp 
GLREF_D TReal64 addInput[];
GLREF_D TReal64 subInput[];
GLREF_D TReal64 multInput[];
GLREF_D TReal64 divInput[];
GLREF_D TReal64 unaryInput[];
GLREF_D TReal64 incDecInput[];
GLREF_D TInt sizeAdd;
GLREF_D TInt sizeSub;
GLREF_D TInt sizeMult;
GLREF_D TInt sizeDiv;
GLREF_D TInt sizeUnary;
GLREF_D TInt sizeIncDec;

LOCAL_D	RFile fw;	// file for WINS
LOCAL_D RFile fa;	// file for ARM
LOCAL_D const TFileName fileNameW64 = _L("T_REAL64.HWG");
LOCAL_D const TFileName fileNameA64 = _L("T_REAL64.HAG");

LOCAL_C void appendArrayName64(TDesC& aName)
	{
	buf=_L("");
	buf.AppendFormat(_L("const TInt64 %S[] = \n{\n"),&aName);
	fw.Write(buf);
	fa.Write(buf);
	}

LOCAL_C void appendValue64(TReal64* aVal)
	{
	buf=_L("");
	buf.AppendFormat(_L("	TInt64(0x%x,0x%x),\n"),*(((TUint32*)aVal) + 1),*(TUint32*)aVal);
	fw.Write(buf);
	buf=_L("");
	buf.AppendFormat(_L("	TInt64(0x%x,0x%x),\n"),*(TUint32*)aVal,*(((TUint32*)aVal) + 1));
	fa.Write(buf);
	}

LOCAL_C void appendArrayTerm()
	{
	buf=_L("};\n\n");
	fw.Write(buf);
	fa.Write(buf);
	}

LOCAL_C void createAddArray()
//
// Writes an array of results of additions to the header file
//
	{
	TInt ii;
	TReal64 ff;

	appendArrayName64(_L("addArray"));
	for (ii=0; ii<sizeAdd-1; ii++)
		{
		ff=addInput[ii]+addInput[ii+1];
		appendValue64(&ff);
		}
	appendArrayTerm();
	}

LOCAL_C void createSubArray()
//
// Writes an array of results of subtractions to the header file
//
	{
	TInt ii;
	TReal64 ff;

	appendArrayName64(_L("subArray"));
	for (ii=0; ii<sizeSub-1; ii++)
		{
		ff=subInput[ii]-subInput[ii+1];
		appendValue64(&ff);
		}
	appendArrayTerm();
	} 

LOCAL_C void createMultArray()
//
// Writes an array of results of multiplications to the header file
//
	{
	TInt ii;
	TReal64 ff;

	appendArrayName64(_L("multArray"));
	for (ii=0; ii<sizeMult-1; ii++)
		{
		ff=multInput[ii]*multInput[ii+1];
		appendValue64(&ff);
		}
	appendArrayTerm();
	} 

LOCAL_C void createDivArray()
//
// Writes an array of results of divisions to the header file
//
	{
	TInt ii;
	TReal64 ff;

	appendArrayName64(_L("divArray"));
	for (ii=0; ii<sizeDiv-1; ii++)
		{
		if (divInput[ii+1]!=0)
			{
			ff=divInput[ii]/divInput[ii+1];
			appendValue64(&ff);
			}
		}
	appendArrayTerm();
	} 

LOCAL_C void createUnaryArray()
//
// Writes an array of results of unary operations to the header file
//
	{
	TReal64 f;

	appendArrayName64(_L("unaryArray"));
	for (TInt ii=0; ii<sizeUnary; ii++)
		{
		f=-unaryInput[ii];
		appendValue64(&f);
		}
	appendArrayTerm();
	} 

LOCAL_C void createIncDecArrays()
//
// Writes an array of results of pre and post increment and decrement operations to the
// header file
//
	{
	TInt ii;
	TReal64 f;

	// Generate arrays for exact tests

	appendArrayName64(_L("preIncArray1"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		++f;
		appendValue64(&f);
		}
	appendArrayTerm();

	appendArrayName64(_L("preIncArray2"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		++f;
		++f;
		appendValue64(&f);
		}
	appendArrayTerm();

	appendArrayName64(_L("preDecArray1"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		--f;
		appendValue64(&f);
		}
	appendArrayTerm();

	appendArrayName64(_L("preDecArray2"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		--f;
		--f;
		appendValue64(&f);
		}
	appendArrayTerm();

	appendArrayName64(_L("postIncArray1"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		f++;
		appendValue64(&f);
		}
	appendArrayTerm();

	appendArrayName64(_L("postIncArray2"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		f++;
		f++;
		appendValue64(&f);
		}
	appendArrayTerm();

	appendArrayName64(_L("postDecArray1"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		f--;
		appendValue64(&f);
		}
	appendArrayTerm();
	
	appendArrayName64(_L("postDecArray2"));
	for (ii=0; ii<sizeIncDec; ii++)
		{
		f=incDecInput[ii];
		f--;
		f--;
		appendValue64(&f);
		}
	appendArrayTerm();
	} 

LOCAL_C void createArrays()
//
// Create and append to header file all arrays required for T_R64.CPP tests
//
	{
	createAddArray();
	createSubArray();
	createMultArray();
	createDivArray();
	createUnaryArray();
	createIncDecArrays();
	}
#endif

GLDEF_C void CallTestsL(void)
//
// Generate TReal64s (and special values if required) and write to header files
//
    {

	test.Title();

#ifdef __GENERATE_TR64
	TFileName msg;
	msg=_L("Generating Maths constants to header files:\n ");
	msg+=pathName;
	msg+=fileNameW64;
	msg+=_L(" and ");
	msg+=fileNameA64;
	test.Start(msg);
#endif

	TInt r=TheFs.MkDirAll(pathName);
	test(r==KErrNone || r==KErrAlreadyExists);
	test(TheFs.SetSessionPath(pathName)==KErrNone);

#ifdef __GENERATE_TR64
	test(fw.Replace(TheFs,fileNameW64,EFileWrite)==KErrNone);
	test(fa.Replace(TheFs,fileNameA64,EFileWrite)==KErrNone);
	test.Next(_L("Generating TReal64s"));
	fw.Write(_L("// Generated by \\E32\\TMATH\\T_GEN.MAK for WINS (little-endian doubles)\n\n"));
	fa.Write(_L("// Generated by \\E32\\TMATH\\T_GEN.MAK for ARM (big-endian doubles)\n\n"));
	createArrays();
	fw.Close();
	fa.Close();
#endif

#ifdef __GENERATE_SPECIAL_VALUES
	test(fspw.Replace(TheFs,fileNameWSp,EFileWrite)==KErrNone);
	test(fspa.Replace(TheFs,fileNameASp,EFileWrite)==KErrNone);
	test.Start(_L("Generate header file of special values for use in maths testing"));
	generateSpecialValues();
	test.Next(_L("Done - now closing files"));
	fspw.Close();
	fspa.Close();
#endif


	test.End();

	return;
    }
