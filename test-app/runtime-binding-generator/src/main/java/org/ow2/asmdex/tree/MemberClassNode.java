package org.ow2.asmdex.tree;

import org.ow2.asmdex.ClassVisitor;

/**
 * A node that represents a member class (specific to ASMDEX)
 *
 * @author Pierre Cregut
 */

public class MemberClassNode {

    /**
     * The internal name of an inner class.
     */
    public String name;

    /**
     * The internal name of the class to which the inner class belongs. May
     * be <tt>null</tt>.
     */
    public String outerName;

    /**
     * The (simple) name of the inner class inside its enclosing class. May be
     * <tt>null</tt> for anonymous inner classes.
     */
    public String innerName;



    /**
     * Constructs a new {@link InnerClassNode}.
     *
     * @param name the internal name of an inner class.
     * @param outerName the internal name of the class to which the inner class
     *        belongs. May be <tt>null</tt>.
     * @param innerName the (simple) name of the inner class inside its
     *        enclosing class. May be <tt>null</tt> for anonymous inner
     *        classes.
     */
    public MemberClassNode(
        final String name,
        final String outerName,
        final String innerName) {
        this.name = name;
        this.outerName = outerName;
        this.innerName = innerName;
    }

    /**
     * Makes the given class visitor visit this inner class.
     *
     * @param cv a class visitor.
     */
    public void accept(final ClassVisitor cv) {
        cv.visitMemberClass(name, outerName, innerName);
    }

}
