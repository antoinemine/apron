/*
 * StringVar.java
 *
 * Implementation of a String variable 
 *
 * APRON Library / Java Apron binding
 *
 */
package apron;

public class StringVar implements Var
{
    
    private String name;

    public StringVar(String name)
    {
        this.name = name;
    }
    
    public int compareTo(Var v)
    {
        return toString().compareTo(v.toString());
    }
    
    public boolean equals(Object o)
    {
        return
            (o instanceof StringVar) &&
            name.equals(((StringVar)o).name);
    }
    
    public int hashCode()
    {
        return name.hashCode();
    }
    
    public String toString()
    {
        return name;
    }
    
    public Var clone() {
        return new StringVar(name);
    }
    
    public static Var[] arrayOfStrings(String []vars)
    {
        if (vars==null) {
            return null;
        }
        
        Var vvars[] = new Var[vars.length];
        for(int i=0; i < vars.length; i++) {
            vvars[i] = new StringVar(vars[i]);
        }
        return vvars;
    }
} 
