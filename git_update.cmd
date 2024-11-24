@echo off
echo Pushing to Git repository
git --version
git add .
git commit -m %1
git push
pause