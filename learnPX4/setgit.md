ssh-keygen -t ed25519-sk -C "your_email@example.com"
less ~/.ssh/id_ed25519.pub
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519
ssh -T git@github.com
git remote set-url origin git@github.com:aslanzhao/PX4-Autopilot.git
