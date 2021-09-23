using System;
using System.Collections.Generic;

namespace test {

   delegate object MyDG(params object[] args);

   class Cat {
       public Cat() {
      }

      public static Cat Create(params object[] args) {
         return new Cat();
      }

      public override string ToString() {
         return "no name!";
      }
   }

   class Dog {
      public string Name { get; set; }

      public Dog(string name) {
         this.Name = name;
      }

      public static Dog Create(params object[] args) {
         return new Dog((string)args[0]);
      }

      public override string ToString() {
         return this.Name;
      }
   }

   class Factory {
      static Dictionary<Type, MyDG> list = new Dictionary<Type, MyDG>();

      public static object Create(Type key, params object[] args) {
         if (list.ContainsKey(key) == false) {
            throw new Exception(string.Format("Don't know how to create a \"{0}\"", key));
         }
         return list[key](args);
      }

      public static void AddMaker(Type key, MyDG maker) {
         list.Add(key, maker);
      }

      public static void RemoveMaker(Type key) {
         list.Remove(key);
      }
   }

   class FactoryItem<T> where T : new() {
      public T Create() {
         return new T();
      }
   }



   class Program {
      public static void Main(string[] args) {
         Factory.AddMaker(typeof(Cat), Cat.Create);
         Factory.AddMaker(typeof(Dog), Dog.Create);
         //Factory.RemoveMaker("cat");

         object o1 = Factory.Create(typeof(Cat));
         object o2 = Factory.Create(typeof(Dog), "milou");
         o1.GetType();

         Console.WriteLine(o1);
         Console.WriteLine(o2);

      }
   }
}
