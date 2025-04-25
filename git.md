開發新功能的時候記得開新的branch
git checkout -b <branch_name>
git push -u origin <branch_name>

把上游的main更新拉到自己的repo main
git fetch upstream
git checkout main
git merge upstream/main

