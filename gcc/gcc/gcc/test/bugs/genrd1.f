      program newgenroad
c     generate a road data file with user specified data

      integer ntu               ! road segment terrain unit number
      integer iroad             ! road type
                                ! 1-super highway
                                ! 2-primary road
                                ! 3-secondary road
                                ! 4-trail
      integer ist               ! soil type (used for trails)
                                ! 1-fine grained
                                ! 2-coarse grained
                                ! 6-fine grained,impervious
                                !   to water, eg CH
      integer iurb              ! urban code
      integer rci(4)            ! seasonal soil strengths (trails)
      integer grade             ! road gradient
      integer rda(4)            ! seasonal recognition distances
      integer actrms            ! surface roughness (rms inches)
      integer radc              ! radius of curvature (feet, 9999 straight)
      real    dist              ! unit or segment length (miles)
      real    eang              ! superelevation angle
                                ! degrees, used on radii 

      character *20 fname
      character *80 desc
      logical       onegrade
      integer mingrade,maxgrade,incgrade,nsegs

c     initialise

      iroad = 2
      ist   = 1
      rci(1)= 300
      rci(2)= 300
      rci(3)= 300
      rci(4)= 300
      rda(1)= 500
      rda(2)= 500
      rda(3)= 500
      rda(4)= 500
      iurb  = 4
      grade = 0
      actrms= 1
      eang  = 0.0
      dist  = 0.0311   ! 50m
      radc  = 9999

c     begin

      print*,'genroad......'
      print*,'generate road data for'
      print*,'the Mobility model'
      print*,' '
      print*,'you can set..'
      print*,'the road type'
      print*,'the grade'
      print*,'the unit length'

      print*,'what road type would you like?'
      print*,'Super highway  --> 1'
      print*,'Primary road   --> 2'
      print*,'Secondary road --> 3'
      print*,'Trail          --> 4'
      print*,' '
      print*,'enter code for road type'
      read(5,*)iroad

      print*,'You can have constant' 
      print*,'or increasing grade'
      print*,'enter 1 for constant grade'
      print*,'any other number for inc grade'
      read(5,*)i
      if(i.eq.1)then
         print*,'using constant grade'
         onegrade=.TRUE.
         else
            print*,'using increasing grade'
            onegrade=.FALSE.
            endif
            
      if(onegrade)then
         print*,'integer value for grade?'
         read(5,*)grade
         print*,'how many road segments?'
         read(5,*)nsegs
         else
            print*,'min grade (integer)?'
            read(5,*)mingrade
            print*,'max grade (integer)?'
            read(5,*)maxgrade
            print*,'increment in grade (int)?'
            read(5,*)incgrade
            endif

      print*,'road segment lengths (metres)?'
      read(5,*)dist
      dist = dist*3.28/5280.0   ! metres to miles

      print*,'give a clear description'
      print*,'of the road file contents'
      print*,'80 char max, used as header'
      read(5,'(a)')desc
      
      print*,' '
      print*,'output filename?'
      read(5,'(a)')fname

      open(unit=1,file=fname)
      write(1,1)desc
 1    format(a80)

 2    format(i5,3i2,10i4,i5,f8.4,f5.0)

      if(onegrade)then
         do i = 1 , nsegs
         write(1,2)i,iroad,ist,iurb,rci,grade,rda,actrms,radc,dist,eang
         write(6,2)i,iroad,ist,iurb,rci,grade,rda,actrms,radc,dist,eang
         enddo
         else
            ntu = 0
            do grade = mingrade,maxgrade,incgrade
               ntu = ntu + 1
               write(1,2)ntu,iroad,ist,iurb,rci,grade,rda,actrms
     &                   ,radc,dist,eang
               write(6,2)ntu,iroad,ist,iurb,rci,grade,rda,actrms
     &                   ,radc,dist,eang
               enddo
      endif         
      close(1)

      stop
      end
