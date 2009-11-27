--- src/foecommand.h.orig	2009-11-13 13:18:32.685464000 +0000
+++ src/foecommand.h	2009-11-13 13:18:55.590464000 +0000
@@ -36,10 +36,10 @@
   
   virtual void doChangeDirection(double d);
   virtual void doChangeSpeed(double s);
-  virtual void FoeCommand::doAccelX(double ax);
-  virtual void FoeCommand::doAccelY(double ay);
-  virtual double FoeCommand::getBulletSpeedX();
-  virtual double FoeCommand::getBulletSpeedY();
+  virtual void doAccelX(double ax);
+  virtual void doAccelY(double ay);
+  virtual double getBulletSpeedX();
+  virtual double getBulletSpeedY();
   
  private:
   struct foe *foe;
