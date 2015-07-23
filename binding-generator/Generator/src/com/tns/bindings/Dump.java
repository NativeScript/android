package com.tns.bindings;

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.*;

import org.ow2.asmdex.*;
import org.ow2.asmdex.structureCommon.*;

import android.util.Log;

public class Dump
{
	public static final char CLASS_NAME_LOCATION_SEPARATOR = '_';
	
	private static final String callJsMethodSignatureCtor = "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Z[Ljava/lang/Object;";
	private static final String callJsMethodSignatureMethod = "Ljava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Ljava/lang/Class;[Ljava/lang/Object;";
	private static final String LCOM_TNS = "Lcom/tns/gen/";
	private static final String LCOM_TNS_PLATFORM = "Lcom/tns/Platform;";
	static final String preferenceActivityJniSignature = "Lcom/tns/android/preference/PreferenceActivity;";
	static final String preferenceActivityClass = "Landroid/preference/PreferenceActivity;";
	static final String objectClass = "Ljava/lang/Object;";

	static final String platformClass = LCOM_TNS_PLATFORM;
	static final String callJSMethodName = "callJSMethod";
	static final String initInstanceMethodName = "initInstance";

	static final StringBuffer methodDescriptorBuilder = new StringBuffer();
	
	
	/**
	 * Returns the dex descriptor corresponding to the given method.
	 * 
	 * @param m
	 *            a {@link Method Method} object.
	 * @return the descriptor of the given method.
	 */
	 public String getDexMethodDescriptor(final Method method) 
	 {
        Class<?>[] parameters = method.getParameterTypes();
        methodDescriptorBuilder.setLength(0);
        getDexDescriptor(methodDescriptorBuilder, method.getReturnType());
        for (int i = 0; i < parameters.length; ++i)
        {
            getDexDescriptor(methodDescriptorBuilder, parameters[i]);
        }
        return methodDescriptorBuilder.toString();
    }
	 
	 /**
	 * Returns the dex descriptor corresponding to the given method.
	 * 
	 * @param m
	 *            a {@link Method Method} object.
	 * @return the descriptor of the given method.
	 */
	 public String getMethodOverloadDescriptor(final Method method) 
	 {
        Class<?>[] parameters = method.getParameterTypes();
        methodDescriptorBuilder.setLength(0);
        for (int i = 0; i < parameters.length; ++i)
        {
            getDexDescriptor(methodDescriptorBuilder, parameters[i]);
        }
        return methodDescriptorBuilder.toString();
    }
	 
	 /**
	 * Returns the dex descriptor corresponding to the given method.
	 * 
	 * @param m
	 *            a {@link Method Method} object.
	 * @return the descriptor of the given method.
	 */
	 public String getDexConstructorDescriptor(final Constructor constructor) 
	 {
	    Class<?>[] parameters = constructor.getParameterTypes();
	    methodDescriptorBuilder.setLength(0);
	    getDexDescriptor(methodDescriptorBuilder, Void.TYPE);
	    for (int i = 0; i < parameters.length; ++i)
	    {
	        getDexDescriptor(methodDescriptorBuilder, parameters[i]);
	    }
	    return methodDescriptorBuilder.toString();
	}

	private void getDexDescriptor(final StringBuffer buf, final Class<?> c)
	{
		Class<?> d = c;
		while (true)
		{
			if (d.isPrimitive())
			{
				char car;
				if (d == Integer.TYPE)
				{
					car = 'I';
				}
				else if (d == Void.TYPE)
				{
					car = 'V';
				}
				else if (d == Boolean.TYPE)
				{
					car = 'Z';
				}
				else if (d == Byte.TYPE)
				{
					car = 'B';
				}
				else if (d == Character.TYPE)
				{
					car = 'C';
				}
				else if (d == Short.TYPE)
				{
					car = 'S';
				}
				else if (d == Double.TYPE)
				{
					car = 'D';
				}
				else if (d == Float.TYPE)
				{
					car = 'F';
				}
				else
				/* if (d == Long.TYPE) */{
					car = 'J';
				}
				buf.append(car);
				return;
			}
			else if (d.isArray())
			{
				buf.append('[');
				d = d.getComponentType();
			}
			else
			{
				buf.append('L');
				String name = d.getName().replace('.', '/');
				buf.append(name);
//				int len = name.length();
//				for (int i = 0; i < len; ++i)
//				{
//					char car = name.charAt(i);
//					buf.append(car == '.' ? '/' : car);
//				}
				buf.append(';');
				return;
			}
		}
	}
    
    /**
     * Returns the descriptor corresponding to the given Java type.
     * From ASM sources org.objectweb.asm.Type
     * @param c
     *            an object class, a primitive class or an array class.
     * @return the descriptor corresponding to the given class.
     */
    public static String getAsmDescriptor(final Class<?> c) {
        StringBuffer buf = new StringBuffer();
        getAsmDescriptor(buf, c);
        return buf.toString();
    }
    
    /**
     * Appends the descriptor of the given class to the given string buffer.
     * From ASM sources org.objectweb.asm.Type
     * @param buf
     *            the string buffer to which the descriptor must be appended.
     * @param c
     *            the class whose descriptor must be computed.
    */
	private static void getAsmDescriptor(final StringBuffer buf, final Class<?> c)
	{
		Class<?> d = c;
		while (true)
		{
			if (d.isPrimitive())
			{
				char car;
				if (d == Integer.TYPE)
				{
					car = 'I';
				}
				else if (d == Void.TYPE)
				{
					car = 'V';
				}
				else if (d == Boolean.TYPE)
				{
					car = 'Z';
				}
				else if (d == Byte.TYPE)
				{
					car = 'B';
				}
				else if (d == Character.TYPE)
				{
					car = 'C';
				}
				else if (d == Short.TYPE)
				{
					car = 'S';
				}
				else if (d == Double.TYPE)
				{
					car = 'D';
				}
				else if (d == Float.TYPE)
				{
					car = 'F';
				}
				else
				/* if (d == Long.TYPE) */{
					car = 'J';
				}
				buf.append(car);
				return;
			}
			else if (d.isArray())
			{
				buf.append('[');
				d = d.getComponentType();
			}
			else
			{
				buf.append('L');
				String name = d.getName().replace('.', '/');
				buf.append(name);
//				int len = name.length();
//				for (int i = 0; i < len; ++i)
//				{
//					char car = name.charAt(i);
//					buf.append(car == '.' ? '/' : car);
//				}
				buf.append(';');
				return;
			}
		}
	}
 
    /**
     * Returns the Class  signature of the Type normalized if it is a primitive 
    */
	private static String getClassSignatureOfType(final Class<?> c)
	{
		final StringBuffer buf = new StringBuffer();
		Class<?> result;
		
		if (c.isPrimitive())
		{
			if (c == Integer.TYPE)
			{
				result = Integer.class;
			}
			else if (c == Void.TYPE)
			{
				result = Void.class;
			}
			else if (c == Boolean.TYPE)
			{
				result = Boolean.class;
			}
			else if (c == Byte.TYPE)
			{
				result = Byte.class;
			}
			else if (c == Character.TYPE)
			{
				result = Character.class;
			}
			else if (c == Short.TYPE)
			{
				result = Short.class;
			}
			else if (c == Double.TYPE)
			{
				result = Double.class;
			}
			else if (c == Float.TYPE)
			{
				result = Float.class;
			}
			else
				/* if (c == Long.TYPE) */
			{
				result = Long.class;
			}
		}
		else if (c.isArray())
		{
			result = Object.class; 
		}
		else
		{
			result = c;
		}
		
		buf.append('L');
		String name = result.getName().replace('.', '/');
		
//		int len = name.length();
//		for (int i = 0; i < len; ++i)
//		{
//			char car = name.charAt(i);
//			buf.append(car == '.' ? '/' : car);
//		}
		buf.append(name);
		buf.append(';');
		
		return buf.toString();
	}
    
    public void generateProxy(ApplicationWriter aw, String proxyName, Class<?> classTo, String[] methodOverrides, int ignored)
    {
    	HashSet<String> methodOverridesSet = new HashSet<String>();
		for (int i = 0; i < methodOverrides.length; i++)
		{
			String methodOverride = methodOverrides[i];
			methodOverridesSet.add(methodOverride);
		}
    	generateProxy(aw, proxyName, classTo, methodOverridesSet);
    }
    
    public void generateProxy(ApplicationWriter aw, Class<?> classTo, String[] methodOverrides, int ignored)
    {
    	HashSet<String> methodOverridesSet = new HashSet<String>();
		for (int i = 0; i < methodOverrides.length; i++)
		{
			String methodOverride = methodOverrides[i];
			methodOverridesSet.add(methodOverride);
		}
    	generateProxy(aw, "0", classTo, methodOverridesSet);
    }
    
    public void generateProxy(ApplicationWriter aw, String proxyName, Class<?> classTo)
	{
    	generateProxy(aw, proxyName, classTo, null);
	}
    
    public void generateProxy(ApplicationWriter aw, Class<?> classTo)
   	{
       	generateProxy(aw, "0", classTo, null);
   	}
    
    public void generateProxy(ApplicationWriter aw, String proxyName, Class<?> classTo, HashSet<String> methodOverrides)
	{
		String classSignature = getAsmDescriptor(classTo);
		//String methodSignature = org.objectweb.asm.Type.getMethodDescriptor(Object.class.getMethods()[0]);
		String tnsClassSignature = LCOM_TNS + 
				classSignature.substring(1, classSignature.length() - 1).replace("$", "_")
				+ CLASS_NAME_LOCATION_SEPARATOR + proxyName + ";";
		
		ClassVisitor cv = generateClass(aw, classTo, classSignature, tnsClassSignature);
		Method[] methods = getSupportedMethods(classTo, methodOverrides);
		
		methods = groupMethodsByName(methods);
		
		generateFields(cv);
		
		Constructor<?>[] ctors = classTo.getConstructors();

		boolean hasOverridenCtor = ((methodOverrides != null) && methodOverrides.contains("init"));
		generateCtors(cv, classTo, ctors, classSignature, tnsClassSignature, hasOverridenCtor);
		generateMethods(cv, classTo, methods, classSignature, tnsClassSignature);
		
		cv.visitEnd();
	}
	
	private Method[] groupMethodsByName(Method[] methods)
	{
		HashMap<String, Method> result = new HashMap<String, Method>();
		for (int i = 0; i < methods.length; i++)
		{
			Method method = methods[i];
			String methodName = method.getName();
			String methodOverLoadDescriptor = getMethodOverloadDescriptor(method);
			methodName +=  "_" + methodOverLoadDescriptor;
			if (!result.containsKey(methodName))
			{
				result.put(methodName, method);
			}
		}
		
		return result.values().toArray(new Method[result.size()]);
	}

	private Method[] getSupportedMethods(Class<?> clazz, HashSet<String> methodOverrides)
	{
		ArrayList<Method> result = new ArrayList<Method>();
		HashMap<String, Method> finalMethods = new HashMap<String, Method>();
		ArrayList<Class<?>> implementedInterfaces = new ArrayList<Class<?>>();
		while (clazz != null)
		{
			Method[] methods = clazz.getDeclaredMethods();
			
			ArrayList<Method> methodz = new ArrayList<Method>();
			
			for (int i = 0; i < methods.length; i++)
			{
				Method candidateMethod = methods[i];
				if (methodOverrides != null && !methodOverrides.contains(candidateMethod.getName()))
				{
					continue;
				}
				
				methodz.add(methods[i]);
			}
			
			
			Class<?>[] interfaces = clazz.getInterfaces();
			for (int i = 0; i < interfaces.length; i++)
			{
				implementedInterfaces.add(interfaces[i]);
			}
			
			for (int i = 0; i < methodz.size(); i++)
			{
				Method method = methodz.get(i);
				
				if (isMethodSupported(method, finalMethods))
				{
					result.add(method);
				}
			}
			
			clazz = clazz.getSuperclass();
		}
		
		for (int i = 0; i < implementedInterfaces.size(); i++)
		{
			Class<?> implementedInterface = implementedInterfaces.get(i);
			Method[] interfaceMethods = implementedInterface.getMethods();
			for (int j = 0; j < interfaceMethods.length; j++)
			{
				Method method = interfaceMethods[j];
				
				if (methodOverrides != null && !methodOverrides.contains(method.getName()))
				{
					continue;
				}
				
				if (!isMethodMarkedAsFinalInClassHierarchy(method, finalMethods))
				{
					result.add(method);	
				}
			}
		}
		
		return result.toArray(new Method[result.size()]);

		//For debugging
		//return new Method[] { result.get(0) };
	}
	
	private static boolean isMethodSupported(Method method, HashMap<String, Method> finalMethods)
	{
		int modifiers = method.getModifiers();
		if (method.isSynthetic() || Modifier.isStatic(modifiers) || Modifier.isPrivate(modifiers))
		{
			return false;
		}
		
		if (Modifier.isFinal(modifiers))
		{
			finalMethods.put(method.getName(), method);
			return false;
		}
		
		boolean isPackagePrivate = !Modifier.isPrivate(modifiers) && !Modifier.isPublic(modifiers) && !Modifier.isProtected(modifiers);
		if (isPackagePrivate)
		{
			return false;
		}
		
		if (isMethodMarkedAsFinalInClassHierarchy(method, finalMethods))
		{
			return false;
		}

//		if (finalMethods.size() != 0)
//		{
//			Method finalMethod = finalMethods.get(method.getName());
//			if (finalMethod != null)
//			{
//				boolean isSameFinalMethod = areMethodSignaturesEqual(finalMethod, method);
//				if (isSameFinalMethod)
//				{
//					return false;
//				}
//			}
//		}
		
		return true;
	}
	
	private static boolean isMethodMarkedAsFinalInClassHierarchy(Method method, HashMap<String, Method> finalMethods)
	{
		if (finalMethods.size() != 0)
		{
			Method finalMethod = finalMethods.get(method.getName());
			if (finalMethod != null)
			{
				boolean isSameFinalMethod = areMethodSignaturesEqual(finalMethod, method);
				if (isSameFinalMethod)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	private static boolean areMethodSignaturesEqual(Method x, Method y)
	{
		if (x.equals(y))
			return true;

		if (!x.getName().equals(y.getName()))
			return false;

		Class<?>[] xParams = x.getParameterTypes();
		Class<?>[] yParams = y.getParameterTypes();

		if (xParams.length != yParams.length)
			return false;

		boolean result = true;

		for (int i = 0; i < xParams.length; i++)
		{
			if (!xParams[i].equals(yParams[i]))
			{
				result = false;
				break;
			}
		}

		return result;
	}

	private void generateCtors(ClassVisitor cv, Class<?> classTo, Constructor<?>[] ctors, String classSignature, String tnsClassSignature, boolean hasOverridenCtor)
	{
		if (classTo.isInterface())
		{
			try
			{
				generateCtor(cv, classTo, Object.class.getConstructor(), classSignature, tnsClassSignature, false);
			}
			catch (NoSuchMethodException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		else
		{
			for (Constructor<?> ctor : ctors)
			{
				generateCtor(cv, classTo, ctor, classSignature, tnsClassSignature, hasOverridenCtor);
			}
		}
	}
	
	
	private void generateCtor(ClassVisitor cv, Class<?> classTo, Constructor<?> ctor, String classSignature, String tnsClassSignature, boolean hasOverridenCtor)
	{
		//TODO: handle generic and vararg ctors if needed
		String ctorSignature = getDexConstructorDescriptor(ctor);
		//org.objectweb.asm.Type.getConstructorDescriptor(ctor);
		
		MethodVisitor mv;
		int ctorModifiers = getDexModifiers(ctor.getModifiers());
		//int locaVarsCount = 2;
		//int thisRegister = locaVarsCount + 1;
		
		mv = cv.visitMethod(ctorModifiers + org.ow2.asmdex.Opcodes.ACC_CONSTRUCTOR, "<init>", ctorSignature, null, null);
		mv.visitCode();
		
		//mv.visitMaxs(4, 0); //TODO: max stack size should be equal to localVarCount + 1
		int thisRegister = generateMaxStackSize(mv, ctor);

		int argCount = ctor.getParameterTypes().length;
		int[] args = generateArgsArray(thisRegister, argCount, ctor);
		
		if (!classTo.isInterface())
		{
			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT_RANGE, classSignature, "<init>", ctorSignature, args);
		}
		else
		{
			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_DIRECT_RANGE, objectClass, "<init>", ctorSignature, args);
		}
		
		generateInitializedBlock(mv, thisRegister, classSignature, tnsClassSignature);
		if (hasOverridenCtor)
		{
			generateCtorOverridenBlock(mv, thisRegister, ctor, classSignature, tnsClassSignature);
		}
		generateReturnVoid(mv);
	}
	
	private void generateReturnVoid(MethodVisitor mv)
	{
		mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
		mv.visitEnd();
	}

	private void generateCtorOverridenBlock(MethodVisitor mv, int thisRegister, Constructor<?> ctor, String classSignature, String tnsClassSignature)
	{
		int argCount = generateArrayForCallJsArguments(mv, ctor.getParameterTypes(), thisRegister, classSignature, tnsClassSignature);
		mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, "init"); //put "init" in register 1
		mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 2, 1); //put true to register 2 == isConstructor argument
		mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, LCOM_TNS_PLATFORM, "callJSMethod", callJsMethodSignatureCtor, new int[]
		{ 3, 1, 2, 0 }); //invoke callJSMethod(this, "init", true, params)
	}

	private void generateInitializedBlock(MethodVisitor mv, int thisRegister, String classSignature, String tnsClassSignature)
	{
 		mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IGET_BOOLEAN, tnsClassSignature, "__initialized", "Z", thisRegister - 2, thisRegister); //put __initialized in local var 1
		Label label = new Label();
		mv.visitJumpInsn(org.ow2.asmdex.Opcodes.INSN_IF_NEZ, label, thisRegister - 2, 0); //compare local var 1 with false
		mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, thisRegister - 1, 1); 		//put true in local var 1 
		mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_IPUT_BOOLEAN, tnsClassSignature, "__initialized", "Z", thisRegister - 1 , thisRegister); //set field to the value of 2
		mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, LCOM_TNS_PLATFORM, "initInstance", "VLjava/lang/Object;", new int[] { thisRegister }); //call init instance passing this as arugment
		mv.visitLabel(label);
	}
	
	private void generateMethods(ClassVisitor cv, Class<?> classTo, Method[] methods, String classSignature, String tnsClassSignature)
	{
		//for (Method method : methods)
		int fieldNameCounter = 0;
		int bitCounter = 1;
		for (int i = 0; i < methods.length; i++)
		{
			if (bitCounter == 128)
			{
				bitCounter = 1;
				fieldNameCounter++;
			}
			
			Method sourceMethod = methods[i];
			
			generateMethod(cv, classTo, sourceMethod, i, classSignature, tnsClassSignature, bitCounter);
			
			bitCounter *= 2;
		}
		
		generateEqualsSuper(cv);
		generateHashCodeSuper(cv);
	}
	
	private void generateEqualsSuper(ClassVisitor cv)
	{
		MethodVisitor mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "equals__super", "ZLjava/lang/Object;", null, null);
		mv.visitCode();
		mv.visitMaxs(3, 0);
		mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "equals", "ZLjava/lang/Object;", new int[] { 1, 2 });
		mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
		mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 0);
		mv.visitEnd(); 
	}
	
	private void generateHashCodeSuper(ClassVisitor cv)
	{
		MethodVisitor mv = cv.visitMethod(org.ow2.asmdex.Opcodes.ACC_PUBLIC, "hashCode__super", "I", null, null);
		mv.visitCode();
		mv.visitMaxs(2, 0);
		mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_SUPER, "Ljava/lang/Object;", "hashCode", "I", new int[] { 1 });
		mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, 0);
		mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, 0);
		mv.visitEnd(); 
	}

	private void generateMethod(ClassVisitor cv, Class<?> classTo, Method method, int methodNumber, String classSignature, String tnsClassSignature, int fieldBit)
	{
		if (ProxyGenerator.IsLogEnabled) Log.d("TNS.Rungime.Proxy.Generator", "generatingMethod " + method.getName());
		
		//TODO: handle checked exceptions
		String methodDexSignature = getDexMethodDescriptor(method);
		String[] exceptions = new String[0];
		
	
		MethodVisitor mv;
		int methodModifiers = getDexModifiers(method.getModifiers());

		mv = cv.visitMethod(methodModifiers, method.getName(), methodDexSignature, null, exceptions);
		mv.visitCode();
		
		int thisRegister = generateMaxStackSize(mv, method);
				
		if (!classTo.isInterface())
		{
			generateInitializedBlock(mv, thisRegister, classSignature, tnsClassSignature);
		}
		generateCallOverrideBlock(mv, method, thisRegister, classSignature, tnsClassSignature, methodDexSignature, fieldBit);

		mv.visitEnd();
	}

	private int generateMaxStackSize(MethodVisitor mv, Method method)
	{
		//3 local vars are enough for NativeScript bindings methods. Local vars start from 0 register till register 2. 
		//Then 'this' is register 3 and then all parameters according to their size
		int registersCount = 3/*local vars*/ + 1 /*this*/;
		int thisRegister = registersCount - 1;
		
		Class<?>[] paramTypes = method.getParameterTypes();
		int paramCount = paramTypes.length;
		
		for (int i = 0; i < paramCount; i++)
		{
			Class<?> paramType = paramTypes[i];
			
			if (paramType == Integer.TYPE || paramType == Character.TYPE || paramType == Byte.TYPE || paramType == Short.TYPE || 
					paramType == Boolean.TYPE || paramType == Float.TYPE)
			{
				registersCount++;
			}
			else if (paramType == Long.TYPE || paramType == Double.TYPE)
			{
				registersCount += 2;
			}
			else 
			{
				registersCount += 1;
			}
		}	
		
		mv.visitMaxs(registersCount, 0);
		
		return thisRegister;
	}
	
	private int generateMaxStackSize(MethodVisitor mv, Constructor<?> ctor)
	{
		//3 local vars are enough for NativeScript bindings methods. Local vars start from 0 register till register 2. 
		//Then 'this' is register 3 and then all parameters according to their size
		int registersCount = 3/*local vars*/ + 1 /*this*/;
		int thisRegister = registersCount - 1;
		
		Class<?>[] paramTypes = ctor.getParameterTypes();
		int paramCount = paramTypes.length;
		
		for (int i = 0; i < paramCount; i++)
		{
			Class<?> paramType = paramTypes[i];
			
			if (paramType == Integer.TYPE || paramType == Character.TYPE || paramType == Byte.TYPE || paramType == Short.TYPE || 
					paramType == Boolean.TYPE || paramType == Float.TYPE)
			{
				registersCount++;
			}
			else if (paramType == Long.TYPE || paramType == Double.TYPE)
			{
				registersCount += 2;
			}
			else 
			{
				registersCount += 1;
			}
		}	
		
		mv.visitMaxs(registersCount, 0);
		
		return thisRegister;
	}

	private void generateCallOverrideBlock(MethodVisitor mv, Method method, int thisRegister, String classSignature, String tnsClassSignature, String methodDexSignature, int fieldBit)
	{
		//call the override
		int argCount = generateArrayForCallJsArguments(mv, method.getParameterTypes() , thisRegister, classSignature, tnsClassSignature);
		mv.visitStringInsn(org.ow2.asmdex.Opcodes.INSN_CONST_STRING, 1, method.getName());
		
		Class<?> returnType = method.getReturnType();
		if (returnType.isPrimitive())
		{
			//mv.visitFieldInsn(INSN_SGET_OBJECT, "Ljava/lang/Long;", "TYPE", "Ljava/lang/Class;", 2, 0);
			mv.visitFieldInsn(org.ow2.asmdex.Opcodes.INSN_SGET_OBJECT, getClassSignatureOfType(returnType), "TYPE", "Ljava/lang/Class;", 2, 0); 
		}
		else
		{
 			mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CONST_CLASS, 2, 0, 0, getClassSignatureOfType(returnType)); 
		}
		
		mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC, platformClass, callJSMethodName, callJsMethodSignatureMethod, new int[] { thisRegister, 1, 2, 0 });
		
		//Label returnLabel = new Label();
		//mv.visitLabel(returnLabel);
		generateReturnFromObject(mv, method.getReturnType(), thisRegister, 1);
 	}

	private int[] generateArgsArray(int thisRegister, int argCount, Constructor ctor)
	{
		Class<?>[] paramTypes = ctor.getParameterTypes();
		int argumentsCount = paramTypes.length;
		int[] argsForSuper = new int[1 + argumentsCount*2]; //thisRegister + argCount * 2 since it long and double take 2 registers

		int argsForSuperIndex = 0;
		argsForSuper[argsForSuperIndex] = thisRegister;
		argsForSuperIndex++;
		
		int arrayIndex = 0;
		while (arrayIndex < argumentsCount)
		{
			Class<?> paramType = paramTypes[arrayIndex];
			if (paramType.isPrimitive())
			{
				if (paramType == Integer.TYPE || paramType == Character.TYPE || paramType == Byte.TYPE || paramType == Short.TYPE || 
						paramType == Boolean.TYPE || paramType == Float.TYPE)
				{
					argsForSuper[argsForSuperIndex] = thisRegister + arrayIndex + 1;
					argsForSuperIndex++;
				}
				else if (paramType == Long.TYPE ||  paramType == Double.TYPE)
				{
					argsForSuper[argsForSuperIndex] = thisRegister + arrayIndex + 1;
					argsForSuperIndex++;
					argsForSuper[argsForSuperIndex] = thisRegister + arrayIndex + 2;
					argsForSuperIndex++;
				}
			}
			else
			{
				argsForSuper[argsForSuperIndex] = thisRegister + arrayIndex + 1;
				argsForSuperIndex++;
			}

			arrayIndex++;
		}
		
		return Arrays.copyOf(argsForSuper, argsForSuperIndex);
	}
	
	/**
	 * 	Creates new Object[] or null value (when no arguments) and puts it in register 0  
	 */
	private int generateArrayForCallJsArguments(MethodVisitor mv, Class<?>[] paramTypes, int thisRegister, String classSignature, String tnsClassSignature)
	{
		int argumentsCount = paramTypes.length;
		
		if (argumentsCount == 0)
		{
			mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_4, 0, 0); //set null at register 0. our params array is null
			return 0;
		}
		
		mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 2, argumentsCount); //put array size in register 2
		mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_NEW_ARRAY, 0, 0, 2, "[Ljava/lang/Object;"); //create array with size in register 2 and put it in register 0
		
		int arrayIndex = 0;
		int argNumber = 4;
		int numberOfDoubleRegisterArguments = 0;
		while (argNumber < 4 + argumentsCount + numberOfDoubleRegisterArguments)
		{
			mv.visitVarInsn(org.ow2.asmdex.Opcodes.INSN_CONST_16, 1, arrayIndex); //put the array access index value in register 1
			
			Class<?> paramType = paramTypes[arrayIndex];
			if (paramType.isPrimitive())
			{
				if (paramType == Integer.TYPE)
				{
					//box the primitive value at register i
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Integer;", "valueOf", "Ljava/lang/Integer;I", new int[] { argNumber });

					//move the result object in register 2
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					
					//put the object at register 2 in array at index register 1
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
				}
				else if (paramType == Character.TYPE)
				{
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Character;", "valueOf", "Ljava/lang/Character;C", new int[] { argNumber });
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

				}
				else if (paramType == Byte.TYPE)
				{
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Byte;", "valueOf", "Ljava/lang/Byte;B", new int[] { argNumber });
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

				}
				else if (paramType == Short.TYPE)
				{
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Short;", "valueOf", "Ljava/lang/Short;S", new int[] { argNumber });
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

				}
				else if (paramType == Boolean.TYPE)
				{
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Boolean;", "valueOf", "Ljava/lang/Boolean;Z", new int[] { argNumber });
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
				}
				else if (paramType == Long.TYPE)
				{
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Long;", "valueOf", "Ljava/lang/Long;J", new int[] { argNumber, argNumber + 1 });
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
					argNumber++;
					numberOfDoubleRegisterArguments++;
				}
				else if (paramType == Float.TYPE)
				{
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Float;", "valueOf", "Ljava/lang/Float;F", new int[] { argNumber });
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);

				}
				else if (paramType == Double.TYPE)
				{
					mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_STATIC_RANGE, "Ljava/lang/Double;", "valueOf", "Ljava/lang/Double;D", new int[] { argNumber, argNumber + 1 });
					mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, 2);
					mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, 2, 0, 1);
					argNumber++;
					numberOfDoubleRegisterArguments++;
				}
			}
			else
			{
				//register 1 contains the value of the arrayIndex
				//i is the register containing the value of the object reference
				//0 register contains the reference to the array
				mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, argNumber, 0, 1); 
			}
					
					
			
//			int arrayPutOpCode = getArrayPutInstructionByType(paramTypes[arrayIndex]);
			
			//mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_OBJECT, i, 0, 1); //put params[i] in the array at index register 1
			//mv.visitArrayOperationInsn(org.ow2.asmdex.Opcodes.INSN_APUT_
			argNumber++;
			arrayIndex++;
		}
		
		return argumentsCount;
	}
	
//	private int getArrayPutInstructionByParamType(Class<?> paramType)
//	{
//		if (paramType.isPrimitive())
//		{
//            if (paramType == Integer.TYPE)
//            {
//                return org.ow2.asmdex.Opcodes.INSN_APUT_WIDE;
//            }
//            else if (paramType == Boolean.TYPE)
//            {
//            	return org.ow2.asmdex.Opcodes.INSN_APUT_BOOLEAN;
//            } 
//            else if (d == Byte.TYPE) {
//                car = 'B';
//            } 
//            else if (d == Character.TYPE) {
//                car = 'C';
//            } 
//            else if (d == Short.TYPE) {
//                car = 'S';
//            }
//            else if (d == Double.TYPE) {
//                car = 'D';
//            }
//            else if (d == Float.TYPE) {
//                car = 'F';
//            }
//            else /* if (d == Long.TYPE) */{
//                car = 'J';
//            }
//            buf.append(car);
//            return;
//        } else if (d.isArray()) {
//            buf.append('[');
//            d = d.getComponentType();
//        } else {
//            buf.append('L');
//            String name = d.getName();
//            int len = name.length();
//            for (int i = 0; i < len; ++i) {
//                char car = name.charAt(i);
//                buf.append(car == '.' ? '/' : car);
//            }
//            buf.append(';');
//            return;
//        }
//	}


	private static final String integerTypeDescriptor = "Ljava/lang/Integer;";
	private static final String booleanTypeDescriptor = "Ljava/lang/Boolean;";
	private static final String byteTypeDescriptor = "Ljava/lang/Byte;";
	private static final String characterTypeDescriptor = "Ljava/lang/Character;";
	private static final String shortTypeDescriptor = "Ljava/lang/Short;";
	private static final String doubleTypeDescriptor = "Ljava/lang/Double;";
	private static final String floatTypeDescriptor = "Ljava/lang/Float;";
	private static final String longTypeDescriptor = "Ljava/lang/Long;";
	
	private void generateReturnFromObject(MethodVisitor mv, Class<?> targetReturnType, int thisRegister, int valueRegister)
	{
		if (targetReturnType == Void.TYPE) 
		{
			mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
			return;
		}
		
		mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, valueRegister); //get the result of from the very last method call in register 1;
		if (targetReturnType.isPrimitive()) 
		{
			if (targetReturnType == Integer.TYPE)
            {
            	mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, integerTypeDescriptor); //throw exception if can't cast this reference to the return type
    			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, integerTypeDescriptor, "intValue", "I", new int[] { valueRegister });
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            }
            else if (targetReturnType == Boolean.TYPE) 
            {
    			mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, booleanTypeDescriptor); //throw exception if can't cast this reference to the return type
    			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, booleanTypeDescriptor, "booleanValue", "Z", new int[] { valueRegister });
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            }
            else if (targetReturnType == Byte.TYPE) 
            {
            	mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, byteTypeDescriptor); //throw exception if can't cast this reference to the return type
    			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, byteTypeDescriptor, "byteValue", "B", new int[] { valueRegister });
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            }
            else if (targetReturnType == Character.TYPE) 
            {
            	mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, characterTypeDescriptor); //throw exception if can't cast this reference to the return type
    			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, characterTypeDescriptor, "charValue", "C", new int[] { valueRegister });
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            }
            else if (targetReturnType == Short.TYPE) 
            {
            	mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, shortTypeDescriptor); //throw exception if can't cast this reference to the return type
    			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, shortTypeDescriptor, "shortValue", "S", new int[] { valueRegister });
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            }
            else if (targetReturnType == Float.TYPE) 
            {
            	mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, floatTypeDescriptor); //throw exception if can't cast this reference to the return type
    			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, floatTypeDescriptor, "floatValue", "F", new int[] { valueRegister });
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister);
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
            }
            else if (targetReturnType == Double.TYPE) 
            {
            	mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, doubleTypeDescriptor); //throw exception if can't cast this reference to the return type
            	mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, doubleTypeDescriptor, "doubleValue", "D", new int[] { valueRegister });
            	mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_WIDE, valueRegister);
            	mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_WIDE, valueRegister);
            }
            else //if (d == Long.TYPE)
            {
            	mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, longTypeDescriptor); //throw exception if can't cast this reference to the return type
    			mv.visitMethodInsn(org.ow2.asmdex.Opcodes.INSN_INVOKE_VIRTUAL, longTypeDescriptor, "longValue", "J", new int[] { valueRegister });
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_WIDE, valueRegister);
    			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_WIDE, valueRegister);
            }
        } 
		else //return object
		{
			String returnTypeDescriptor = getAsmDescriptor(targetReturnType);
			mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, returnTypeDescriptor); //throw exception if can't cast this reference to the return type
			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, valueRegister);
		}
	}
	
	/**
	 * Generate return instructions from targetReturnType. TargetReturnType 
	 * @param mv
	 * @param targetReturnType
	 * @param thisRegister
	 * @param valueRegister
	 */
	private void generateReturn(MethodVisitor mv, Class<?> targetReturnType, int thisRegister, int valueRegister)
	{
		if (targetReturnType == Void.TYPE) 
		{
			mv.visitInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_VOID);
		}
		else if (targetReturnType.isPrimitive() && targetReturnType != Double.TYPE && targetReturnType != Long.TYPE) 
		{
			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT, valueRegister); //get the result of from the very last method call in register 1;
			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN, valueRegister);
        }
		else if (targetReturnType.isPrimitive() && (targetReturnType == Double.TYPE || targetReturnType == Long.TYPE)) 
		{
			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_WIDE, valueRegister); //get the result of from the very last method call in register 1;
			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_WIDE, valueRegister);
        } 
		else //return object
		{
			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_MOVE_RESULT_OBJECT, valueRegister); //get the result of from the very last method call in register 1;
			String returnTypeDescriptor = getAsmDescriptor(targetReturnType);
			mv.visitTypeInsn(org.ow2.asmdex.Opcodes.INSN_CHECK_CAST, 0, valueRegister, 0, returnTypeDescriptor); //throw exception if can't cast this reference to the return type
			mv.visitIntInsn(org.ow2.asmdex.Opcodes.INSN_RETURN_OBJECT, valueRegister);
		}
	}

	private void generateFields(ClassVisitor cv)
	{
		generateInitializedField(cv);
	}

	private void generateInitializedField(ClassVisitor cv)
	{
		FieldVisitor fv = cv.visitField(org.ow2.asmdex.Opcodes.ACC_PRIVATE, "__initialized", "Z", null, null);
		fv.visitEnd();
	}

	static final String[] classImplentedInterfaces = new String[] { "Lcom/tns/NativeScriptHashCodeProvider;" };
	static final String[] interfaceImplementedInterfaces = new String[] { "Lcom/tns/NativeScriptHashCodeProvider;", "" };
	private ClassVisitor generateClass(ApplicationWriter aw, Class<?> classTo, String classSignature, String tnsClassSignature)
	{
		ClassVisitor cv;
		
		int classModifiers = getDexModifiers(classTo.getModifiers());
		String[] implentedInterfaces = classImplentedInterfaces;
		if (classTo.isInterface())
		{
			interfaceImplementedInterfaces[1] = classSignature; //new String[] { "Lcom/tns/NativeScriptHashCodeProvider;", classSignature };
			implentedInterfaces = interfaceImplementedInterfaces;
			classSignature = objectClass;
		}
		else
		{
			implentedInterfaces = classImplentedInterfaces;
		}
		
		cv = aw.visitClass(classModifiers, tnsClassSignature, null, classSignature, implentedInterfaces);
		cv.visit(0, classModifiers, tnsClassSignature, null, classSignature, implentedInterfaces);
		cv.visitSource(classTo.getName() +  ".java", null);
		return cv;
	}

	private int getDexModifiers(int modifiers)
	{
		if (Modifier.isPublic(modifiers))
		{
			return org.ow2.asmdex.Opcodes.ACC_PUBLIC;
		}
		
		return org.ow2.asmdex.Opcodes.ACC_PROTECTED;
	}
}
